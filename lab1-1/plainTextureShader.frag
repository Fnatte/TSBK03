#version 150

in vec2 outTexCoord;
uniform sampler2D image;
uniform sampler2D bloom;
out vec4 out_Color;

void main(void) {
    out_Color = texture(image, outTexCoord) + texture(bloom, outTexCoord);
}
