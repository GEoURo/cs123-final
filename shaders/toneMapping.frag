#version 330 core

in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D hdrBuffer;

uniform float exposure;
uniform bool hdrEnabled;

void main(void)
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, texCoord).rgb;
    if (!hdrEnabled) {
        fragColor = vec4(hdrColor, 1.0);
    } else {
        // reinhard
        // vec3 result = hdrColor / (hdrColor + vec3(1.0));
        // exposure
        vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
        // also gamma correct while we're at it
        result = pow(result, vec3(1.0 / gamma));
        fragColor = vec4(result, 1.0);
    }
}
