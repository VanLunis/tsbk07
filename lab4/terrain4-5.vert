#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 texCoord;
out vec3 exNormal;
out vec3 surf;
out vec3 pixelPos;

// NY
uniform mat4 projMatrix;
uniform mat4 mdlMatrix;
uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];
uniform vec3 camPos;

void main(void)
{
	mat3 normalMatrix1 = mat3(mdlMatrix);
	texCoord = inTexCoord;
	gl_Position = projMatrix * mdlMatrix * vec4(inPosition, 1.0);
	exNormal = normalize(normalMatrix1 * inNormal);
	surf = inPosition; //For specular
	pixelPos = inPosition;
}
