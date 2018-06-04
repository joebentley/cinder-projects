#version 400
#define M_PI 3.1415926535897932384626433832795

uniform float uAnim;
uniform vec3 uLightCoord;
out vec4 oColor;

in VertexData {
    float Offset;
    vec3 Position;
} VertexIn;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(void) {
    vec3 X = dFdx(VertexIn.Position);
    vec3 Y = dFdy(VertexIn.Position);
    vec3 normal = cross(X, Y);

    vec3 color = hsv2rgb(vec3(mod(VertexIn.Offset + 0.1 * uAnim, 1.0), 1.0, 1.0));
    color *= clamp(dot(normalize(uLightCoord), normalize(normal)), 0.1, 1.0);
    oColor = vec4(color, 1);
}