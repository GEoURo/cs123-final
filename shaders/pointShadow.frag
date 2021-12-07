#version 330 core
in vec4 fragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main(void) {
    // get distance between fragment and light source
    float lightDistance = length(fragPos.xyz - lightPos);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / farPlane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}
