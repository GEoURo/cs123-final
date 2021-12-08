#version 330 core

// fragment shader input in world space
in vec4 fragPos;
in vec4 fragNormal;
in vec2 texCoords;

// fragment shader output
out vec4 fragColor;

struct CS123Settings {
    bool useShadow;
    bool useTexture;
};

// global data struct
struct CS123GlobalData {
    float ka;
    float kd;
    float ks;
};

// material data struct
struct CS123Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    float blend;
    float repeatUV;
    sampler2D diffuseTexture;
};

// point shadow map struct
struct CS123PointShadowMap {
    int lightId;
    int farPlane;
    samplerCube depthMap;
};

// directional shadow map struct
struct CS123DirectionalShadowMap {
    int lightId;
    mat4 lightSpaceMat;
    sampler2D depthMap;
};

// settings
uniform CS123Settings settings;

// scene global data
uniform CS123GlobalData global;

// primitive material
uniform CS123Material material;

// point shadow
uniform CS123PointShadowMap pointShadow;

// directional shadow
uniform CS123DirectionalShadowMap directionalShadow;

// position of camera
uniform vec4 cameraPos;

// Light data
const int MAX_LIGHTS = 10;
uniform int lightTypes[MAX_LIGHTS];         // 0 for point, 1 for directional
uniform vec3 lightPositions[MAX_LIGHTS];    // For point lights
uniform vec3 lightDirections[MAX_LIGHTS];   // For directional lights
uniform vec3 lightAttenuations[MAX_LIGHTS]; // Constant, linear, and quadratic term
uniform vec3 lightColors[MAX_LIGHTS];

void main() {
    vec3 color = material.ambient * global.ka;

    for (int i = 0; i < MAX_LIGHTS; i++) {
        vec4 vertexToLight = vec4(0);

        // The attenuation coefficient
        float attenuation = 1.f;

        // The shadow coefficient
        float shadow = 0.f;

        if (lightTypes[i] == 0) {
            // Point Light
            float d = length(vec4(lightPositions[i], 1) - fragPos);
            attenuation = min(1.f, 1.f / (lightAttenuations[i].x + d * lightAttenuations[i].y + d * d * lightAttenuations[i].z));
            vertexToLight = normalize(vec4(lightPositions[i], 1) - fragPos);
        } else if (lightTypes[i] == 1) {
            // Dir Light
            vertexToLight = normalize(vec4(-lightDirections[i], 0));
        } else {
            // ignore the light
            continue;
        }

        // prepare diffuse and specular color
        vec3 cDiffuse = global.kd * material.diffuse;
        vec3 cSpecular = material.shininess == 0 ? vec3(0) : global.ks * material.specular;

        if (settings.useTexture) {
            // sample the texture color
            vec2 coords = texCoords * material.repeatUV;
            vec3 cTexture = texture(material.diffuseTexture, texCoords).rgb;

            // blend the texture color with diffuse color
            cDiffuse = mix(cDiffuse, cTexture, material.blend);
        }

        // Add diffuse component
        float diffuseIntensity = max(0.0, dot(vertexToLight, fragNormal));
        cDiffuse = max(vec3(0), attenuation * lightColors[i] * cDiffuse * diffuseIntensity);

        // Add specular component
        vec4 lightReflection = normalize(reflect(-vertexToLight, fragNormal));
        vec4 eyeDirection = normalize(cameraPos - fragPos);
        float specIntensity = pow(max(0.0, dot(eyeDirection, lightReflection)), material.shininess);
        cSpecular = max(vec3(0), attenuation * lightColors[i] * cSpecular * specIntensity);

        color += (1.0 - shadow) * (cDiffuse + cSpecular);
    }

    // clamp the color that are smaller than 0
    color = max(vec3(0.f), color);

    // set the output value
    fragColor = vec4(color, 1);
}
