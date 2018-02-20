#version 150
/*

in vec2 outTexCoord;
in vec3 shading;

uniform sampler2D tex;

out vec4 out_Color;

void main(void)
{
	out_Color = vec4(shading, 1.0);


*/
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
	//exTexCoord3 = vec3(exTexCoord,1.0);
	//float a = sin(exTexCoord3.s*30 * (1.0+sin(t/2.0)))/2 + 0.5;
	//float b = sin(exTexCoord.t * 30.0 * (1.0+sin(t/4.0)))/2.0 + 0.5;
	//out_Color = vec4(a, b, 1.0, 0.0);
	//out_Color = texture(textUnit, exTexCoord);
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
}
	out_Color = vec4(diffuseStrength*0.5 + specularStrength*0.5);

}
