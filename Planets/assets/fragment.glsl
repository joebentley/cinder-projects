#version 400

in vec4 Color;
out vec4 oColor;

uniform vec3 uLightCoord;

in VertexData {
    vec3 Normal;
} VertexIn;

void main() {
    vec3 c = Color.xyz;
    c *= clamp(dot(uLightCoord, VertexIn.Normal), 0.1, 1);
    
    oColor = vec4(c, 1.0);
}
