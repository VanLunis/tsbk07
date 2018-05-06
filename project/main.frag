#version 150
in vec3 frag_Color;
in vec2 exTexCoord;
in vec3 exNormal;
in vec3 surf;
in vec3 worldNormal;
in vec3 pixelPos;
in vec4 fixdPos;

out vec4 out_Color;
out vec4 out_Normal;
out vec3 exTexCoord3;

uniform float t;
uniform sampler2D planetTex;
uniform float sunBool;
uniform vec3 SunPos;
uniform vec3 lightSourcesColorArr;
uniform float specularExponent;
uniform bool isDirectional;
uniform vec3 camPos;

void main(void)
{

	vec3 lightDirr = SunPos - vec3(fixdPos);
	vec3 shade = vec3(0,0,0);
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);

	exTexCoord3 = vec3(exTexCoord,1.0);





	//diffuse
	float tmp_diff = 0;
	tmp_diff = dot(worldNormal, lightDirr);
	tmp_diff = max(0.0, tmp_diff); // No negative light
	diffuse += tmp_diff * lightSourcesColorArr;


	//Specular
	vec3 r;
	float tmp_spec;
	if (isDirectional)
	{
		r = reflect(lightDirr, worldNormal);
	}
	else
	{
		r = reflect(normalize(surf-lightDirr), worldNormal);
	}

	vec3 v = normalize(camPos-surf);

	tmp_spec = dot(r,v);
	if(tmp_spec > 0.0)
	{
		tmp_spec = pow(tmp_spec, specularExponent);
	}
	tmp_spec = max(tmp_spec, 0.0);
	specular += tmp_spec *lightSourcesColorArr;


	//Total
	if (sunBool ==	1) //if sun
	{
		shade = vec3(1,1,1);
	}
	else // all planets
	{
		shade = clamp(0.2*diffuse + 0.005*specular, vec3(0.0, 0.0, 0.0), vec3(1.0,1.0,1.0));
	}

	vec4 shadePart = vec4(shade, 1.0);
	vec4 texPart = texture(planetTex,exTexCoord.st);

	out_Color.r = texPart.r * shadePart.r;
	out_Color.g = texPart.g * shadePart.g;
	out_Color.b = texPart.b * shadePart.b;
}
