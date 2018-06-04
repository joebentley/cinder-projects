#version 400

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in VertexData {
    vec3 Position;
    float Offset;
} VertexIn[3];

out VertexData {
    float Offset;
    vec3 Position;
} VertexOut;

void main() {
    for (int i = 0; i < gl_in.length(); ++i) {
        gl_Position = gl_in[i].gl_Position;
        VertexOut.Offset = VertexIn[i].Offset;
        VertexOut.Position = VertexIn[i].Position;
        EmitVertex();
    }
    EndPrimitive();
}
