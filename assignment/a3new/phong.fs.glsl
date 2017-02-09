varying vec4 V_Normal_VCS;
varying vec4 V_ViewPosition;
uniform vec3 lightColor;
uniform vec3 ambientColor;
uniform vec4 lightDirection;
uniform float kAmbient;
uniform float kDiffuse;
uniform float kSpecular;
uniform float shininess;
varying mat4 projMat;
varying mat4 modViewMat;
varying mat3 norMat;

void main() {
    
    vec3 V_color = vec3(1.0, 0.0, 0.0);
    
    vec3 lightVector = normalize(vec3(lightDirection));
    vec3 viewVector = normalize(vec3(projMat*modViewMat*V_ViewPosition));
    vec3 normalVector = normalize(norMat * vec3(V_Normal_VCS));
    vec3 lightReflect = normalize(reflect(-lightVector, normalVector));
    
    float specular = pow(max(0.0, dot(lightReflect, viewVector)), shininess);
    float diffuse = max(0.0, dot(normalVector, lightVector));
    
    V_color = kAmbient * ambientColor+
            diffuse * lightColor * kDiffuse+
            lightColor * specular * kSpecular;
    

	// COMPUTE LIGHTING HERE
	gl_FragColor = vec4(V_color, 1.0);
}