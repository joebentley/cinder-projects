#version 400

uniform mat4 ciModelViewProjection;
in vec4 ciPosition;

in vec4 ciColor;
out vec4 Color;

in vec3 ciNormal;
out vec3 Normal;

uniform float uAnim;

void main(void) {
    vec4 pos = ciPosition;
    
    float angle = atan(pos.z, pos.x);
    pos.y += 0.2 * (sin(5 * angle + 4 * uAnim) + sin(5 * angle - 4 * uAnim));
    
    gl_Position = ciModelViewProjection * pos;
    Color = ciColor;
    Normal = ciNormal;
}
