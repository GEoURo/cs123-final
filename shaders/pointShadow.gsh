#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMatrices[6];

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    // layer 0
    gl_Layer = 0;
    FragPos = gl_in[0].gl_Position;
    gl_Position = shadowMatrices[0] * FragPos;
    EmitVertex();

    FragPos = gl_in[1].gl_Position;
    gl_Position = shadowMatrices[0] * FragPos;
    EmitVertex();

    FragPos = gl_in[2].gl_Position;
    gl_Position = shadowMatrices[0] * FragPos;
    EmitVertex();
    EndPrimitive();

    // layer 1
    gl_Layer = 1;
    FragPos = gl_in[0].gl_Position;
    gl_Position = shadowMatrices[1] * FragPos;
    EmitVertex();

    FragPos = gl_in[1].gl_Position;
    gl_Position = shadowMatrices[1] * FragPos;
    EmitVertex();

    FragPos = gl_in[2].gl_Position;
    gl_Position = shadowMatrices[1] * FragPos;
    EmitVertex();
    EndPrimitive();

    // layer 2
    gl_Layer = 2;
    FragPos = gl_in[0].gl_Position;
    gl_Position = shadowMatrices[2] * FragPos;
    EmitVertex();

    FragPos = gl_in[1].gl_Position;
    gl_Position = shadowMatrices[2] * FragPos;
    EmitVertex();

    FragPos = gl_in[2].gl_Position;
    gl_Position = shadowMatrices[2] * FragPos;
    EmitVertex();
    EndPrimitive();

    // layer 3
    gl_Layer = 3;
    FragPos = gl_in[0].gl_Position;
    gl_Position = shadowMatrices[3] * FragPos;
    EmitVertex();

    FragPos = gl_in[1].gl_Position;
    gl_Position = shadowMatrices[3] * FragPos;
    EmitVertex();

    FragPos = gl_in[2].gl_Position;
    gl_Position = shadowMatrices[3] * FragPos;
    EmitVertex();
    EndPrimitive();

    // layer 4
    gl_Layer = 4;
    FragPos = gl_in[0].gl_Position;
    gl_Position = shadowMatrices[4] * FragPos;
    EmitVertex();

    FragPos = gl_in[1].gl_Position;
    gl_Position = shadowMatrices[4] * FragPos;
    EmitVertex();

    FragPos = gl_in[2].gl_Position;
    gl_Position = shadowMatrices[4] * FragPos;
    EmitVertex();
    EndPrimitive();

    // layer 5
    gl_Layer = 5;
    FragPos = gl_in[0].gl_Position;
    gl_Position = shadowMatrices[5] * FragPos;
    EmitVertex();

    FragPos = gl_in[1].gl_Position;
    gl_Position = shadowMatrices[5] * FragPos;
    EmitVertex();

    FragPos = gl_in[2].gl_Position;
    gl_Position = shadowMatrices[5] * FragPos;
    EmitVertex();
    EndPrimitive();
}
