#version 150
in vec3 frag_Color;
in vec2 exTexCoord;
in vec3 exNormal;
in vec3 surf;
in vec3 worldNormal;
in vec3 pixelPos;

out vec4 out_Color;
out vec4 out_Normal;
out vec3 exTexCoord3;

uniform float t;
uniform sampler2D dirtTex;
uniform sampler2D rutorTex;

uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];
uniform vec3 camPos;

void main(void)
{
	//const vec3 light = vec3(0.58, 0.58, 0.58);
	vec3 shade = vec3(0,0,0);
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);
	//shade = dot(normalize(exNormal), light);
	//shade = clamp(shade, 0, 1);
	//exTexCoord3 = vec3(exTexCoord,1.0);
	//float a = sin(exTexCoord3.s*30 * (1.0+sin(t/2.0)))/2 + 0.5;
	//float b = sin(exTexCoord.t * 30.0 * (1.0+sin(t/4.0)))/2.0 + 0.5;
	//out_Color = vec4(a, b, 1.0, 0.0);
	//out_Color = texture(textUnit, exTexCoord);
	//out_Color = vec4(shade, shade, shade, 1.0);




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
		r = reflect(normalize(surf-lightSourcesDirPosArr[i]), worldNormal);
	}

	vec3 v = normalize(camPos-surf); // Reverse view direction

	tmp_spec = dot(r, v);
	if (tmp_spec > 0.0)
	{
		tmp_spec = pow(tmp_spec, specularExponent);
	}
	tmp_spec = max(tmp_spec, 0.0);
	specular += tmp_spec * lightSourcesColorArr[i];
}



	//vec2 tempTexCoord = vec2(100*exTexCoord.s, 100*exTexCoord.t);
	shade = 0.3*diffuse + specular;
	vec4 shadePart = vec4(shade, 1.0);
	vec4 dirtPart = sin(pixelPos.x*5.0) * texture(dirtTex,100*exTexCoord.st);
	vec4 rutorPart = (1-sin(pixelPos.x*5.0)) * texture(rutorTex,100*exTexCoord.st);
	out_Color = vec4(shade, 1.0) * (dirtPart +  rutorPart);
	//out_Color = vec4(shade, 1.0) * ( texture(dirtTex, 0.01*exTexCoord.st) * sin(pixelPos.x*5) + texture(rutorTex,0.01*exTexCoord.st) * (1-sin(pixelPos.x*5)) );
	//out_Color = ( texture(dirtTex, 100*exTexCoord.st) * sin(pixelPos.x*5) + texture(rutorTex,100*exTexCoord.st) * (1-sin(pixelPos.x*5)) );





}
