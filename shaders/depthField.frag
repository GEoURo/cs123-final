#version 330 core

in vec2 texCoord;
out vec4 fragColor;

uniform sampler2D colorTexture;
uniform sampler2D depthTexture;
uniform float depthOfField;

uniform float exposure;
uniform float zNear;
uniform float zFar;

void main(void) {
    vec2 texSize  = textureSize(colorTexture, 0).xy;
    vec2 texCoord = gl_FragCoord.xy / texSize;

    fragColor = texture(colorTexture, texCoord);
    int size = 3;
    float separation =  0.3;
    fragColor.rgb = vec3(0);
    float count = 0.0;
    float depth = texture(depthTexture, texCoord).r;
    float zdepth = 2.0 * depth - 1.0;
    float zLinear = 2.0 * zNear * zFar / (zFar + zNear - zdepth * (zFar - zNear));
    float range = 1;

    if (zLinear<=depthOfField-range || zLinear >= depthOfField + range){
        float x = abs(depthOfField-zLinear)-range;
        separation = min(separation * x, 3);

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
    }else{
        fragColor = texture(colorTexture, texCoord);
    }


    fragColor.rgb += depth*0.01;


}
