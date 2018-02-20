#version 150
in vec3 frag_Color;
in vec2 exTexCoord;
in vec3 exNormal;
in vec3 surf;

out vec4 out_Color;
out vec4 out_Normal;
out vec3 exTexCoord3;

uniform float t;
uniform sampler2D textUnit;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	shade = dot(normalize(exNormal), light);
	shade = clamp(shade, 0, 1);
	out_Color = vec4(shade, shade, shade, 1.0);



//Specular

vec3 reflectedLightDirection = reflect(-light, exNormal);
vec3 eyeDirection = normalize(-surf);
float specularStrength = 0.0;
float diffuseStrength = 0.5;
if (dot(light, exNormal) > 0.0)
{
	specularStrength = dot(reflectedLightDirection, eyeDirection);
	float exponent = 5.0;
	specularStrength = max(specularStrength, 0.01);
	specularStrength = pow(specularStrength, exponent);
	specularStrength = 0;
}
	out_Color = vec4(diffuseStrength*0.5 + specularStrength*0.5);
}
