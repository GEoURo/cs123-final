#version 330 core

// fragment shader input in world space
in vec4 fragPos;
in vec4 fragNormal;
in vec2 texCoords;

// fragment shader output
out vec4 fragColor;

// settings
uniform bool useTexture;
uniform bool useShadow;

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
uniform float pointLightFarPlane;
uniform samplerCube pointLightShadowMap;

// Material data
uniform vec3 ambient_color;
uniform vec3 diffuse_color;
uniform vec3 specular_color;
uniform float shininess;
uniform float blend;
uniform vec2 repeatUV;

// diffuse texture map
uniform sampler2D diffuseTexture;

float directionShadowCalculation(vec4 position) {
    // perform perspective divide
    vec3 projCoords = position.xyz / position.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    //check z value
    if(projCoords.z > 1.0){
        projCoords.z = 1.0;
    }

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    //float closestDepth = texture(dirLightShadowMap, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    //add bias
    vec3 lightDir = lightDirections[dirLightID];
    lightDir = normalize(lightDir);
    float dotLightNormal = dot(lightDir, vec3(fragNormal));
    float bias = max(0.05*(1.0-dotLightNormal),0.005);

    // check whether current frag pos is in shadow
    //float shadow = (currentDepth + bias) > closestDepth  ? 1.0 : 0.0;

    //percentage-closer filter
    float shadow = 0.0;
    vec2 texelSize = 1.0/textureSize(dirLightShadowMap,0);
    for(int x=-1;x<=1;x++){
        for(int y=-1;y<=1;y++){
            float closestDepth = texture(dirLightShadowMap, projCoords.xy+vec2(x,y)*texelSize).r;
            shadow+= (currentDepth + bias) > closestDepth  ? 1.0 : 0.0;
        }
    }

    //return shadow;
    return shadow/9.0;
}

float pointShadowCalculation(vec4 position) {
    // calculate the vector from light to fragment position
    vec3 lightToPos = position.xyz - lightPositions[pointLightID];

    // sample from the cube map to retrieve depth info
    float closestDepth = texture(pointLightShadowMap, lightToPos).r;
    // transform the depth from normalized value to actual value
    closestDepth *= pointLightFarPlane;

    float currentDepth = length(lightToPos);
    float bias = 0.05;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    return shadow;
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
            float d = length(vec4(lightPositions[i], 1) - fragPos);
            attenuation = min(1.f, 1.f / (lightAttenuations[i].x + d * lightAttenuations[i].y + d * d * lightAttenuations[i].z));
            vertexToLight = normalize(vec4(lightPositions[i], 1) - fragPos);
            // point light shadow
            if (useShadow && i == pointLightID) {
                // only calculate shadow for a designated point light
                shadow = pointShadowCalculation(fragPos);
            }
        } else if (lightTypes[i] == 1) {
            // Dir Light
            vertexToLight = normalize(vec4(-lightDirections[i], 0));
            // directional light shadow
            if (useShadow && i == dirLightID) {
                // only calculate shadow for a designated directional light
                shadow = directionShadowCalculation(fragPos);
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
            vec3 texColor = texture(diffuseTexture, texCoords).rgb;

            // blend the texture color with diffuse color
            diffuse = mix(diffuse, texColor, blend);
        }

        // Add diffuse component
        float diffuseIntensity = max(0.0, dot(vertexToLight, fragNormal));
        diffuse = max(vec3(0), attenuation * lightColors[i] * diffuse * diffuseIntensity);

        // Add specular component
        vec4 lightReflection = normalize(reflect(-vertexToLight, fragNormal));
        vec4 eyeDirection = normalize(cameraPos - fragPos);
        float specIntensity = pow(max(0.0, dot(eyeDirection, lightReflection)), shininess);
        specular = max(vec3(0), attenuation * lightColors[i] * specular * specIntensity);

        color += (1.0 - shadow) * (diffuse + specular);
    }

    // clamp the color that are smaller than 0
    color = max(vec3(0.f), color);

    // set the output value
    fragColor = vec4(color, 1);
}
