#version 400

uniform mat4 ciModelViewProjection;
in vec4 ciPosition;
in vec3 ciNormal;

out VertexData {
    vec3 Normal;
} VertexOut;

void main() {
    VertexOut.Normal = ciNormal;
    gl_Position = ciModelViewProjection * ciPosition;
}
