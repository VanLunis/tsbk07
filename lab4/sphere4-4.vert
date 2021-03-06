#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec3 exNormal;
out vec3 surf;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform mat4 sphereTranslate;

void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	texCoord = inTexCoord;
	gl_Position = projMatrix * mdlMatrix * sphereTranslate * vec4(inPosition, 1.0);
	exNormal = normalMatrix1 * inNormal;
	surf = inPosition; //For specular
}
