#version 330 core

out vec4 fragColor;

in vec4 fragPos;
in vec4 fragNormal;
in vec2 texCoords;

// Light data
const int MAX_LIGHTS = 10;
uniform int lightTypes[MAX_LIGHTS];         // 0 for point, 1 for directional
uniform vec3 lightPositions[MAX_LIGHTS];    // For point lights
uniform vec3 lightDirections[MAX_LIGHTS];   // For directional lights
uniform vec3 lightAttenuations[MAX_LIGHTS]; // Constant, linear, and quadratic term
uniform vec3 lightColors[MAX_LIGHTS];

uniform int pointLightID;
uniform float pointLightFarPlane;
uniform samplerCube pointLightShadowMap;

void main(void)
{
    vec3 color = vec3(0);
    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (lightTypes[i] == 0 && i == pointLightID) {
            vec3 lightToFrag = fragPos.xyz - lightPositions[pointLightID];
            float closestDepth = texture(pointLightShadowMap, lightToFrag).r;
            color = vec3(closestDepth);
        }
    }
    fragColor = vec4(color, 1);
}
