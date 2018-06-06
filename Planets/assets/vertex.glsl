#version 400

uniform mat4 ciModelViewProjection;
in vec4 ciPosition;
in vec3 ciNormal;
in vec4 ciColor;
out vec4 Color;

out VertexData {
    vec3 Normal;
} VertexOut;

void main() {
    VertexOut.Normal = ciNormal;
    gl_Position = ciModelViewProjection * ciPosition;
    Color = ciColor;
}
