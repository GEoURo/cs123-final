#version 330 core

// fragment shader input
in vec4 fragPos;
in vec4 fragNormal;
in vec2 texCoords;

// fragment shader output
out vec4 fragColor;

uniform sampler2D tex;
uniform bool useTexture;

// global data
uniform float ka;
uniform float kd;
uniform float ks;

// Transformation matrices
uniform mat4 p;
uniform mat4 v;
uniform mat4 m;

// position of camera
uniform vec4 cameraPos;

// Light data
const int MAX_LIGHTS = 10;
uniform int lightTypes[MAX_LIGHTS];         // 0 for point, 1 for directional
uniform vec3 lightPositions[MAX_LIGHTS];    // For point lights
uniform vec3 lightDirections[MAX_LIGHTS];   // For directional lights
uniform vec3 lightAttenuations[MAX_LIGHTS]; // Constant, linear, and quadratic term
uniform vec3 lightColors[MAX_LIGHTS];

// Material data
uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform float shininess;
uniform float blend;
uniform vec2 repeatUV;

void main(){
    vec3 color = ambient_color.xyz * ka;

    for (int i = 0; i < MAX_LIGHTS; i++) {
        vec4 vertexToLight = vec4(0);

        // The attenuation coefficient
        float attenuation = 1.f;

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
        vec3 diffuse = kd * diffuse_color;
        vec3 specular = shininess == 0 ? vec3(0) : ks * specular_color;

        if (useTexture) {
            // sample the texture color
            vec3 texColor = texture(tex, texCoords).rgb;

            // blend the texture color with diffuse color
            diffuse = mix(diffuse, texColor, blend);
        }

        // Add diffuse component
        float diffuseIntensity = max(0.0, dot(vertexToLight, fragNormal));
        color += max(vec3(0), attenuation * lightColors[i] * diffuse * diffuseIntensity);

        // Add specular component
        vec4 lightReflection = normalize(reflect(-vertexToLight, fragNormal));
        vec4 eyeDirection = normalize(cameraPos - fragPos);
        float specIntensity = pow(max(0.0, dot(eyeDirection, lightReflection)), shininess);
        color += max(vec3(0), attenuation * lightColors[i] * specular * specIntensity);
    }

    // clamp the color that are smaller than 0
    color = max(vec3(0.f), color);

    // set the output value
    fragColor = vec4(color, 1);
}
