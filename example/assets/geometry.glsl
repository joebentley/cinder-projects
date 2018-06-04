#version 400

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in VertexData {
    vec3 Position;
    float Offset;
} VertexIn[3];

out VertexData {
    float Offset;
    vec3 Normal;
} VertexOut;

int imod(int a, int b) {
    return int(a - b * floor(a / b));
}

void main() {
    const int len = gl_in.length();

    for (int i = 0; i < len; ++i) {
        gl_Position = gl_in[i].gl_Position;
        VertexOut.Offset = VertexIn[i].Offset;
        vec3 a = VertexIn[imod(i + 1, len)].Position - VertexIn[i].Position;
        vec3 b = VertexIn[imod(i + 2, len)].Position - VertexIn[imod(i + 1, len)].Position;
        VertexOut.Normal = normalize(cross(a, b));
        EmitVertex();
    }
    EndPrimitive();
}
