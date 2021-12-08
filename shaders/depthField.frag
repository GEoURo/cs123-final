#version 330 core

in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D colorTexture;

uniform float exposure;
uniform float gamma;
uniform bool hdrEnabled;

void main(void) {
    vec2 texSize  = textureSize(colorTexture, 0).xy;
    vec2 texCoord = gl_FragCoord.xy / texSize;

    fragColor = texture(colorTexture, texCoord);
    int size = 3;
    float separation =  10;
    fragColor.rgb = vec3(0);
    float count = 0.0;


    for (int i = -size; i <= size; ++i) {
        for (int j = -size; j <= size; ++j) {
            fragColor.rgb +=
                texture
            ( colorTexture
            ,   ( gl_FragCoord.xy
                + (vec2(i, j) * separation)
                )
              / texSize
            ).rgb;

        count += 1.0;
      }
    }

    fragColor.rgb /= count;




}
