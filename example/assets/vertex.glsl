#version 400
#define M_PI 3.1415926535897932384626433832795

uniform mat4 ciModelViewProjection;
in vec4 ciPosition;
in vec2 ciTexCoord0;
out vec2 TexCoord0;
uniform float uAnim;
out float Offset;
out vec3 Position;

float offset(vec2 uv) {
    return (sin(30 * sqrt(pow(0.5 - uv.x, 2) + pow(0.5 - uv.y, 2)) + M_PI * uAnim) + 2 * sin(uv.x * 15.0 - M_PI * uAnim)
    + sin(uv.x * 10.0 + 0.01 * M_PI * uAnim) + cos(uv.y * 7.0 + uv.x * 13 - 0.3 * M_PI * uAnim)) * 0.1;
}

void main(void) {
    vec4 pos = ciPosition;
    Offset = offset(ciTexCoord0);
    pos.y = Offset;
    Position = pos.xyz;
    gl_Position = ciModelViewProjection * pos;
    TexCoord0 = ciTexCoord0;
}