#version 150


in vec2 texCoord;
in vec3 exNormal;
in vec3 surf;

out vec4 outColor;


uniform sampler2D tex;



void main(void)
{
	outColor = texture(tex, texCoord);
/*
 			// Phongshading /
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	shade = dot(normalize(exNormal), light);
	shade = clamp(shade, 0, 1);

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
		outColor = vec4(diffuseStrength*0.5 + specularStrength*0.5);
		*/
}
