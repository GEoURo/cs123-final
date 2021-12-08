#version 330 core

// fragment shader input in world space
in vec4 fragPos;
in vec4 fragNormal;
in vec2 texCoords;

// fragment shader output
out vec4 fragColor;

// point shadow map struct
struct CS123PointShadowMap {
    int lightId;
    int farPlane;
    samplerCube depthMap;
};

// point shadow
uniform CS123PointShadowMap pointShadow;

// Light data
const int MAX_LIGHTS = 10;
uniform int lightTypes[MAX_LIGHTS];         // 0 for point, 1 for directional
uniform vec3 lightPositions[MAX_LIGHTS];    // For point lights
uniform vec3 lightDirections[MAX_LIGHTS];   // For directional lights
uniform vec3 lightAttenuations[MAX_LIGHTS]; // Constant, linear, and quadratic term
uniform vec3 lightColors[MAX_LIGHTS];

void main() {
    vec3 color = vec3(0);

    for (int i = 0; i < MAX_LIGHTS; i++) {
        if (i == pointShadow.lightId) {
            vec3 fragToLight = fragPos.xyz - lightPositions[i];
            float closestDepth = texture(pointShadow.depthMap, fragToLight).r;
            color = vec3(closestDepth);
        }
    }

    // set the output value
    fragColor = vec4(color, 1);
}
