#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 inTexCoords;

out vec2 TexCoords;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main() {
    gl_Position =  MVP * vec4(pos,1);

    TexCoords = inTexCoords;
}