#version 150
// bump mapping should be calculated
// 1) in view coordinates
// 2) in texture coordinates

in vec2 outTexCoord;
in vec3 out_Normal;
in vec3 Ps;
in vec3 Pt;
in vec3 pixPos;  // Needed for specular reflections
uniform sampler2D texUnit;
out vec4 out_Color;

void main(void)
{
	vec3 light = vec3(0.0, 0.7, 0.7); // Light source in view coordinates

	// Calculate gradients here
	float offset = 1.0 / 256.0; // texture size, same in both directions
	float deltaX = (texture(texUnit, vec2(outTexCoord.x + offset, outTexCoord.y)) -
	                texture(texUnit, vec2(outTexCoord.x - offset, outTexCoord.y))) * 2;
	float deltaY = (texture(texUnit, vec2(outTexCoord.x, outTexCoord.y + offset)) -
	                texture(texUnit, vec2(outTexCoord.x, outTexCoord.y - offset))) * 2;

	vec3 normal = normalize(vec3(-deltaX, -deltaY, 1));
	mat3 mvt = transpose(mat3(Ps, Pt, out_Normal));

	out_Color = vec4(dot(normal, mvt * light));
}
