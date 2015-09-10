#version 150

uniform sampler2D texUnit;
uniform float texSize;
in vec2 outTexCoord;
out vec4 outColor;

void main(void)
{
	float offset = 2.0 / texSize;

	vec4 c = texture(texUnit, outTexCoord);
	tc.x = tc.x + offset;
	vec4 l = texture(texUnit, outTexCoord);
	tc.x = tc.x - 2.0 * offset;
	vec4 r = texture(texUnit, outTexCoord);
	tc.x = tc.x - offset;

	out_Color = (c + c + l + r) * 0.25;
}
