#version 400

uniform sampler2D uTex0;
in vec2 TexCoord0;
out vec4 oColor;

void main(void) {
    oColor = texture(uTex0, TexCoord0);
}
