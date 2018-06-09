#version 400

uniform vec3 uLightCoord;

in vec3 Normal;
in vec4 Color;
out vec4 oColor;

void main(void) {
    vec3 c = Color.xyz;
    c *= clamp(0.2 + dot(normalize(Normal), normalize(uLightCoord)), 0.2, 1.0);
    oColor = vec4(c, 1);
}
