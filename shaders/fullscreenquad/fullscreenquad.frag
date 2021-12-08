#version 330 core

in vec2 texCoord;

uniform sampler2D colorAttachment;

out vec4 fragColor;

void main() {
    fragColor = texture(colorAttachment, texCoord);
}
