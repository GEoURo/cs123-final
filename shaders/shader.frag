#version 330 core

// fragment shader input in world space
in VS_OUT {
    vec4 fragPos;
    vec4 fragNormal;
    vec2 texCoords;
} fs_in;

// fragment shader output
out vec4 fragColor;

// settings
uniform bool useTexture;
uniform bool useShadow;

// diffuse texture map
uniform sampler2D tex;

// global data
uniform float ka;
uniform float kd;
uniform float ks;

// position of camera
uniform vec4 cameraPos;

// Light data
const int MAX_LIGHTS = 10;
uniform int lightTypes[MAX_LIGHTS];         // 0 for point, 1 for directional
uniform vec3 lightPositions[MAX_LIGHTS];    // For point lights
uniform vec3 lightDirections[MAX_LIGHTS];   // For directional lights
uniform vec3 lightAttenuations[MAX_LIGHTS]; // Constant, linear, and quadratic term
uniform vec3 lightColors[MAX_LIGHTS];

// directional shadow mapping
uniform int dirLightID;
uniform mat4 dirLightSpaceMat;
uniform sampler2D dirLightShadowMap;

// point shadow mapping
uniform int pointLightID;
uniform mat4 pointLightSpaceMat;
uniform samplerCube pointLightShadowMap;

// Material data
uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform float shininess;
uniform float blend;
uniform vec2 repeatUV;

float directionShadowCalculation() {
    return 0.f;
}

float pointShadowCalculation() {
    return 0.f;
}

void main(){
    vec3 color = ambient_color.xyz * ka;

    for (int i = 0; i < MAX_LIGHTS; i++) {
        vec4 vertexToLight = vec4(0);

        // The attenuation coefficient
        float attenuation = 1.f;
        float shadow = 0.f;

        if (lightTypes[i] == 0) {
            // Point Light
            float d = length(vec4(lightPositions[i], 1) - fs_in.fragPos);
            attenuation = min(1.f, 1.f / (lightAttenuations[i].x + d * lightAttenuations[i].y + d * d * lightAttenuations[i].z));
            vertexToLight = normalize(vec4(lightPositions[i], 1) - fs_in.fragPos);
            // TODO: point light shadow
            if (i == pointLightID) {
                // only calculate shadow for a designated point light
                shadow = pointShadowCalculation();
            }
        } else if (lightTypes[i] == 1) {
            // Dir Light
            vertexToLight = normalize(vec4(-lightDirections[i], 0));
            // TODO: directional light shadow
            if (i == dirLightID) {
                // only calculate shadow for a designated directional light
                shadow = directionShadowCalculation();
            }

        } else {
            // ignore the light
            continue;
        }

        // prepare diffuse and specular color
        vec3 diffuse = kd * diffuse_color;
        vec3 specular = shininess == 0 ? vec3(0) : ks * specular_color;

        if (useTexture) {
            // sample the texture color
            vec3 texColor = texture(tex, fs_in.texCoords).rgb;

            // blend the texture color with diffuse color
            diffuse = mix(diffuse, texColor, blend);
        }

        // Add diffuse component
        float diffuseIntensity = max(0.0, dot(vertexToLight, fs_in.fragNormal));
        diffuse = max(vec3(0), attenuation * lightColors[i] * diffuse * diffuseIntensity);

        // Add specular component
        vec4 lightReflection = normalize(reflect(-vertexToLight, fs_in.fragNormal));
        vec4 eyeDirection = normalize(cameraPos - fs_in.fragPos);
        float specIntensity = pow(max(0.0, dot(eyeDirection, lightReflection)), shininess);
        specular = max(vec3(0), attenuation * lightColors[i] * specular * specIntensity);

        color += (1.0 - shadow) * (diffuse + specular);
    }

    // clamp the color that are smaller than 0
    color = max(vec3(0.f), color);

    // set the output value
    fragColor = vec4(color, 1);
}
