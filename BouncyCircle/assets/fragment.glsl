#version 400

in vec4 Color;
out vec4 oColor;

in vec3 Normal;

uniform vec3 uLightCoord;

void main(void) {
    vec3 c = Color.xyz;
    c *= clamp(0.2 + dot(Normal, -uLightCoord), 0.2, 1.0);
    oColor = vec4(c, 1.0);
}
