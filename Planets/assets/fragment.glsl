#version 400

out vec4 oColor;

in VertexData {
    vec3 Normal;
} VertexIn;

void main() {
    vec3 c = vec3(0, 1, 0);
    
    c *= clamp(dot(normalize(vec3(2, 3, 2)), VertexIn.Normal), 0, 1);
    
    oColor = vec4(c, 1.0);
}
