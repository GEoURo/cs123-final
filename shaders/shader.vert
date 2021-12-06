#version 330 core

layout(location = 0) in vec3 position; // Position of the vertex
layout(location = 1) in vec3 normal;   // Normal of the vertex
layout(location = 5) in vec2 texCoord; // UV texture coordinates
layout(location = 10) in float arrowOffset; // Sideways offset for billboarded normal arrows

out VS_OUT {
    vec4 fragPos;
    vec4 fragNormal;
    vec2 texCoords;
} vs_out;

// Transformation matrices
uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

uniform vec2 repeatUV;

uniform bool useArrowOffsets; // True if rendering the arrowhead of a normal for Shapes

void main() {
    // calculate the actual texture UV after repeating UV times
    vs_out.fragPos = m * vec4(position, 1.f);
    vs_out.fragNormal = vec4(normalize(mat3(transpose(inverse(m))) * normal), 0);
    vs_out.texCoords = texCoord * repeatUV;

    gl_Position = p * v * vs_out.fragPos;
}
