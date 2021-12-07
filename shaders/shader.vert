#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 1) in vec3 normal;   // Normal of the vertex
layout(location = 5) in vec2 texCoord; // UV texture coordinates

out vec4 fragPos;
out vec4 fragNormal;
out vec2 texCoords;

// Transformation matrices
uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

uniform vec2 repeatUV;

void main() {
    // calculate the actual texture UV after repeating UV times
    fragPos = m * vec4(position, 1.f);
    fragNormal = vec4(normalize(mat3(transpose(inverse(m))) * normal), 0);
    texCoords = texCoord * repeatUV;

    gl_Position = p * v * fragPos;
}
