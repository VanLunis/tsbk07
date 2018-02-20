#version 150


in vec2 texCoord;
in vec3 exNormal;
in vec3 surf;
in vec3 pixelPos;

out vec4 outColor;


uniform sampler2D tex;
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];
uniform vec3 camPos;



void main(void)
{

	vec3 shade = vec3(0,0,0);
	vec3 diffuse = vec3(0,0,0);
	vec3 specular = vec3(0,0,0);
	const vec3 light = vec3(0.58, 0.58, 0.58);

	//Diffuse
	//outColor = texture(tex, texCoord);

	float tmp_diff = 0;
	for(int i = 0; i < 4; i++)
	{
		tmp_diff = dot(exNormal, lightSourcesDirPosArr[i]);
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
			r = reflect(lightSourcesDirPosArr[i], exNormal);
		}
		else
		{
			r = reflect(normalize(surf-lightSourcesDirPosArr[i]), exNormal);
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
 			/* Phongshading */
	/*float shade;
	shade = dot(normalize(exNormal), light);
	shade = clamp(shade, 0, 1);
	outColor = vec4(shade, shade, shade, 1.0);

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
		specularStrength  = 0;
	}*/

		shade = 0.1*diffuse + specular;
		outColor = vec4(shade, 1.0);

		//Water!
		if ( (pixelPos.x > 49) && (pixelPos.x < 141) && (pixelPos.z > 70) && (pixelPos.x < 141) && (pixelPos.y < 1) )
		{
				outColor = vec4(0.0, 0.0, 1.0, 0.3);
		}

}
