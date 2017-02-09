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

	// ADJUST THESE VARIABLES TO PASS PROPER DATA TO THE FRAGMENTS
	V_Normal_VCS = vec4(1.0,0.0,0.0, 1.0);
	V_ViewPosition = vec4(1.0,0.0,0.0, 1.0);
    V_Normal_VCS = vec4(normal, 1.0);
    V_ViewPosition = vec4(position, 1.0);
    
    projMat = projectionMatrix;
    modViewMat = modelViewMatrix;
    norMat = normalMatrix;
    
    
    
    
    
    

	gl_Position = projectionMatrix *  modelViewMatrix * vec4(position, 1.0);
}