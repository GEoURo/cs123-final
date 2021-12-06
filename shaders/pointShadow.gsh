#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main() {
    for(int face = 0; face < 6; ++face)
    {
        // built-in variable that specifies to which face we render.
        gl_Layer = face;
        // for each triangle vertex
        for(int i = 0; i < 3; ++i) {
            // set the fragment position to be passed the fragment shader
            FragPos = gl_in[i].gl_Position;
            // transform the vertex from the world space to each light space
            gl_Position = shadowMatrices[face] * FragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}
