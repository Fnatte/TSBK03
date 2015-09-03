/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in vec3 vertNormal;

uniform mat4 viewMatrix;

out vec4 out_Color;

void main(void) {
	float shade = (mat3(viewMatrix) * vertNormal); // Fake shading
	out_Color=vec4(shade, shade, shade, 1.0);
}
