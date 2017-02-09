// The uniform variable is set up in the javascript code and the same for all vertices
uniform vec3 remotePosition;

void main() {
	/* HINT: WORK WITH remotePosition HERE! */
	
	
	  gl_Position.value.x -= remotePosition.value.x;

    // Multiply each vertex by the model-view matrix and the projection matrix to get final vertex position (model-view transform)
    gl_Position = projectionMatrix * modelViewMatrix * vec4(position, 1.0);
}
