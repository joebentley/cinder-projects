#version 400

in vec4 Color;
out vec4 oColor;

in VertexData {
    vec3 Normal;
} VertexIn;

void main() {
    vec3 c = Color.xyz;
    c *= clamp(dot(normalize(vec3(2, 3, 2)), VertexIn.Normal), 0.1, 1);
    
    oColor = vec4(c, 1.0);
}
