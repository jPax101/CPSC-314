#include "object.hpp"

#include <cmath>
#include <cfloat>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>


bool Object::intersect(Ray ray, Intersection &hit) const 
{
    // Assert the correct values of the W coords of the origin and direction.
    // You can comment this out if you take great care to construct the rays
    // properly.
    ray.origin[3] = 1;
    ray.direction[3] = 0;

    Ray local_ray(i_transform * ray.origin, i_transform * ray.direction);
	//!!! USEFUL NOTES: to calculate depth in localIntersect(), if the intersection happens at
	//ray.origin + ray.direction * t, then t is just the depth
	//!!! USEFUL NOTES: Here direction might be scaled, so you mustn't renormalize it in
	//localIntersect(), or you will get a depth in local coordinate system,
	//which can't be compared with intersections with other objects
    if (localIntersect(local_ray, hit)) 
	{
        // Assert correct values of W.
        hit.position[3] = 1;
        hit.normal[3] = 0;
        
		// Transform intersection coordinates into global coordinates.
        hit.position = transform * hit.position;
        hit.normal = (n_transform * hit.normal).normalized();
        
		return true;
    }

    return false;
}


bool Sphere::localIntersect(Ray const &ray, Intersection &hit) const 
{
    // @@@@@@ YOUR CODE HERE
	// You could also use pure geometry relations rather than 
	//analytical tools shown in the slides
	// Here in local coordinate system, the sphere is centered on (0, 0, 0)
	//with radius 1.0

	//
	// NOTE: hit.depth is the current closest intersection depth, so don't
	// accept any intersection that happens further away than that.

	double a = ray.direction[0] * ray.direction[0] + ray.direction[1] * ray.direction[1]
		+ ray.direction[2] * ray.direction[2];
	double b = 2 * (ray.direction[0] * ray.origin[0] + ray.direction[1] * ray.origin[1]
		+ ray.direction[2] * ray.origin[2]);
	double c = ray.origin[0] * ray.origin[0] + ray.origin[1] * ray.origin[1] +
		ray.origin[2] * ray.origin[2] - radius*radius;
	double delta = b*b - 4 * a*c;

	if (delta == 0) {
		double depth = -b / (2 * a);
		hit.depth = depth;
		hit.normal = (ray.origin + depth * ray.direction).normalized();
		hit.position = ray.origin + depth * ray.direction;
		return true;
	} else if (delta > 0){
		double d1 = (-b + sqrt(delta)) / (2 * a);
		double d2 = (-b - sqrt(delta)) / (2 * a);
		if (d1 > 0 && d2 > 0 && d2< hit.depth) {
			hit.depth = d2;
			hit.normal = (ray.origin + d2 * ray.direction).normalized();
			hit.position = ray.origin + d2 * ray.direction;
			return true;
		} else if (d1>0 && d2 <= 0 && d1<hit.depth) {
			hit.depth = d2;
			hit.normal = (ray.origin + d1 * ray.direction).normalized();
			hit.position = ray.origin + d1 * ray.direction;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}


bool Plane::localIntersect(Ray const &ray, Intersection &hit) const
{
	// @@@@@@ YOUR CODE HERE
	// Do not accept intersection while ray is inside the plane
	// Here in local coordinate system, the plane is at z = 0
	//
	// NOTE: hit.depth is the current closest intersection depth, so don't
	// accept any intersection that happens further away than that.
    
    if (!(fabs(ray.direction[2]) < 0.0000001)) {
        
        double depth;
        depth = Vector(1.0-ray.origin[0],1.0-ray.origin[1],-ray.origin[2]).dot(Vector(0.0,0.0,1.0))
        /ray.direction.dot(Vector(0.0,0.0,1.0));
        
        if (depth <= hit.depth && depth > 0) {
            hit.depth = depth;
            hit.normal = Vector(0.0,0.0,1.0);
            hit.position = ray.origin + depth * ray.direction;
            
            return true;
        }
    }
    return false;
}


bool Conic::localIntersect(Ray const &ray, Intersection &hit) const {
    // @@@@@@ YOUR CODE HERE (creative license)
    return false;
}


// Intersections!
bool Mesh::localIntersect(Ray const &ray, Intersection &hit) const
{
	// Bounding box check
	double tNear = -DBL_MAX, tFar = DBL_MAX;
	for (int i = 0; i < 3; i++) {
		if (ray.direction[i] == 0.0) {
			if (ray.origin[i] < bboxMin[i] || ray.origin[i] > bboxMax[i]) {
				// Ray parallel to bounding box plane and outside of box!
				return false;
			}
			// Ray parallel to bounding box plane and inside box: continue;
		}
		else {
			double t1 = (bboxMin[i] - ray.origin[i]) / ray.direction[i];
			double t2 = (bboxMax[i] - ray.origin[i]) / ray.direction[i];
			if (t1 > t2) std::swap(t1, t2); // Ensure t1 <= t2

			if (t1 > tNear) tNear = t1; // We want the furthest tNear
			if (t2 < tFar) tFar = t2; // We want the closest tFar

			if (tNear > tFar) return false; // Ray misses the bounding box.
			if (tFar < 0) return false; // Bounding box is behind the ray.
		}
	}
	// If we made it this far, the ray does intersect the bounding box.

	// The ray hits the bounding box, so check each triangle.
	bool isHit = false;
	for (size_t tri_i = 0; tri_i < triangles.size(); tri_i++) {
		Triangle const &tri = triangles[tri_i];

		if (intersectTriangle(ray, tri, hit)) {
			isHit = true;
		}
	}
	return isHit;
}

double Mesh::implicitLineEquation(double p_x, double p_y,
	double e1_x, double e1_y,
	double e2_x, double e2_y) const
{
	return (e2_y - e1_y)*(p_x - e1_x) - (e2_x - e1_x)*(p_y - e1_y);
}

bool Mesh::intersectTriangle(Ray const &ray,
	Triangle const &tri,
	Intersection &hit) const
{
	// Extract vertex positions from the mesh data.
	Vector const &p0 = positions[tri[0].pi];
	Vector const &p1 = positions[tri[1].pi];
	Vector const &p2 = positions[tri[2].pi];

	// @@@@@@ YOUR CODE HERE
	// Decide whether ray intersects the triangle (p0,p1,p2).
	// If so, fill in intersection information in hit and return true.
	// You may find it useful to use the routine implicitLineEquation()
	// to compute the result of the implicit line equation in 2D.
	//
	// NOTE: hit.depth is the current closest intersection depth, so don't
	// accept any intersection that happens further away than that.
	//!!! USEFUL NOTES: for the intersection point, its normal should satisfy hit.normal.dot(ray.direction) < 0

	/*Vector N = (p1 - p0).cross(p2 - p0);
	double depth = Vector(p0[0] - ray.origin[0], p0[1] - ray.origin[1], p0[2]-ray.origin[2]).dot(N)
		/ ray.direction.dot(N);
	Vector P = ray.origin + depth * ray.direction;

	Vector edge0 = p1 - p0;
	Vector edge1 = p2 - p1;
	Vector edge2 = p0 - p2;
	Vector c0 = P - p0;
	Vector c1 = P - p1;
	Vector c2 = P - p2;

	if (!fabs(ray.direction.dot(N)) < 0.000001 && depth > 0 && depth<hit.depth
		&& N.dot(edge0.cross(c0)) >= 0
		&& N.dot(edge1.cross(c2)) >= 0 
		&& N.dot(edge2.cross(c2)) >= 0) {
		hit.depth = depth;
		hit.position = ray.direction + depth * ray.direction;
		hit.normal = N;
		return true;
	}*/

	return false;
}
