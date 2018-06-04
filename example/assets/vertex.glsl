#version 400
#define M_PI 3.1415926535897932384626433832795

uniform float uAnim;
uniform mat4 ciModelViewProjection;
in vec4 ciPosition;
in vec2 ciTexCoord0;
out float Offset;
out vec3 Normal;

float offset(vec2 uv) {
    return (sin(30 * sqrt(pow(0.5 - uv.x, 2) + pow(0.5 - uv.y, 2)) + M_PI * uAnim) + 2 * sin(uv.x * 15.0 - M_PI * uAnim)
    + sin(uv.x * 10.0 + 0.01 * M_PI * uAnim) + cos(uv.y * 7.0 + uv.x * 13 - 0.3 * M_PI * uAnim)) * 0.1;
}

void main(void) {
    vec4 pos = ciPosition;
    Offset = offset(ciTexCoord0);
    pos.y = Offset;
    gl_Position = ciModelViewProjection * pos;

    float d = 0.0001;
    float dx = (offset(ciTexCoord0 + vec2(d, 0)) - offset(ciTexCoord0)) / d;
    float dz = (offset(ciTexCoord0 + vec2(0, d)) - offset(ciTexCoord0)) / d;
    Normal = normalize(vec3(-dx, 1, -dz));
}