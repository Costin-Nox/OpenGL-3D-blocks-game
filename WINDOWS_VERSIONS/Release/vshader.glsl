//Costin Ghiocel
//CMPT 361 Assignment 2

#version 130

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
out vec4 color;

// output values that will be interpretated per-fragment
out  vec3 fN;
out  vec3 fE;
out  vec3 fL;

uniform mat4 ModelView;
uniform vec4 LightPosition;
uniform mat4 Projection;


void main() 
{
    fN = vNormal;
    fE = (ModelView*vPosition).xyz;
    fL = LightPosition.xyz;
    
    if( LightPosition.w != 0.0 ) {
	fL = LightPosition.xyz - vPosition.xyz;
    }

    gl_Position = Projection*ModelView*vPosition;

	color = vColor;	
} 