#version 400

uniform sampler2D uTex0;
in vec2 TexCoord0;
out vec4 oColor;

uniform vec3 uLightCoord;

in VertexData {
    vec3 Normal;
} VertexIn;

void main(void) {
    vec4 color = texture(uTex0, TexCoord0);
    
    vec3 c = color.xyz;
    c *= clamp(0.4 + dot(uLightCoord, VertexIn.Normal), 0.2, 1);
    
    oColor = vec4(c, 1.0);
}

