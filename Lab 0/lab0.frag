/*
 * random comment here
 * makes syntax highlight appaer
 * colors like springs sprouts
 */

#version 150

in vec3 vertNormal;

uniform mat4 viewMatrix;

out vec4 out_Color;

const vec3 light = vec3(0.58, -0.58, 0.58);

void main(void) {
	float shade = clamp(dot(light, vertNormal), 0, 1);
	out_Color=vec4(shade, shade, shade, 1.0);
}
