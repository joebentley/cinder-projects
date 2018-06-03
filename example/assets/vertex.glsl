#version 150
#define M_PI 3.1415926535897932384626433832795

uniform mat4 ciModelViewProjection;
in vec4 ciPosition;
in vec2 ciTexCoord0;
out vec2 TexCoord0;
uniform float uAnim;

float offset(vec2 uv) {
    return (sin(uv.x * 15.0 + M_PI * uAnim) + cos(uv.y * 7.0 + uv.x * 13 + 0.3 * M_PI * uAnim)) * 0.1;
}

void main(void) {
    vec4 pos = ciPosition;
    pos.y = offset(ciTexCoord0);
    gl_Position = ciModelViewProjection * pos;
    TexCoord0 = ciTexCoord0;
}