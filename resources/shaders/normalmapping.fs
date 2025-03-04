#version 330 core

// Interpolated values from the vertex shaders
in vec2 fragmentColor;

// Ouput data
out vec3 color;

void main(){

	// Output color = color specified in the vertex shader, 
	// interpolated between all 3 surrounding vertices
	color = vec3(fragmentColor, 1.0f);

}