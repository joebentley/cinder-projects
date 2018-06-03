#version 150
#define M_PI 3.1415926535897932384626433832795

uniform float uAnim;
in vec2 TexCoord0;
out vec4 oColor;
in float Offset;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(void) {
    oColor = vec4(hsv2rgb(vec3(mod(Offset + 0.1 * uAnim, 1.0), 1.0, 1.0)), 1);
}