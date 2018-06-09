#version 400

uniform mat4 ciModelViewProjection;
in vec4 ciPosition;

in vec3 ciNormal;
out vec3 Normal;

in vec4 ciColor;
out vec4 Color;

void main(void) {
    gl_Position = ciModelViewProjection * ciPosition;
    Normal = ciNormal;
    Color = ciColor;
}
