#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 1) in vec3 normal;   // Normal of the vertex
layout(location = 5) in vec2 texCoord; // UV texture coordinates

uniform mat4 model;

void main(void) {
    gl_Position = model * vec4(position, 1.0);
}
