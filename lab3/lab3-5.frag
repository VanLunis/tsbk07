#version 150
in vec3 frag_Color;
in vec2 exTexCoord;
in vec3 exNormal;
in vec3 surf;
in vec3 worldNormal;

out vec4 out_Color;
out vec4 out_Normal;
out vec3 exTexCoord3;

uniform float t;
uniform sampler2D textUnit;
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

void main(void)
{
	//const vec3 light = vec3(0.58, 0.58, 0.58);
	vec3 shade = vec3(0,0,0);
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);
	//shade = dot(normalize(exNormal), light);
	//shade = clamp(shade, 0, 1);


//diffuse

float tmp_diff = 0;
for(int i = 0; i < 4; i++)
{
	tmp_diff = dot(worldNormal, lightSourcesDirPosArr[i]);
	tmp_diff = max(0.0, tmp_diff); // No negative light
	diffuse += tmp_diff * lightSourcesColorArr[i];
}

//Specular

float tmp_spec;
for(int i = 0; i < 4; i++)
{
	vec3 r;

	if (isDirectional[i])
	{
		r = reflect(lightSourcesDirPosArr[i], worldNormal);
	}
	else
	{
		r = reflect(normalize(surface_position-lightSourcesDirPosArr[i]), worldNormal);
	}

	vec3 v = normalize(camera_position-surface_position); // Reverse view direction

	tmp_spec = dot(r, v);
	if (tmp_spec > 0.0)
	{
		tmp_spec = pow(tmp_spec, specularExponent[i]);
	}
	tmp_spec = max(tmp_spec, 0.0);
	specular += tmp_spec * lightSourcesColorArr[i];
}

shade = diffuse + specular;
out_Color = vec4(shade, 1.0);

}
