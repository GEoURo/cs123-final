#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex

uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main(void)
{
    gl_Position = lightSpaceMatrix * model * vec4(position, 1.f);
}
