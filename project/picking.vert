#version 150

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;
in vec2 inTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 lookAtMat;
uniform mat4 transformationMatrix;
uniform mat4 actualPosMatrix;

uniform int planetNumber;


out vec3 surf;
out vec3 worldNormal;
out vec3 pixelPos;

out vec4 fixdPos;

void main(void)
{
	surf = (transformationMatrix * vec4(in_Position, 1.0)).xyz;
	worldNormal = normalize(mat3(transformationMatrix) * in_Normal);
	gl_Position = projectionMatrix * lookAtMat * transformationMatrix * vec4(in_Position, 1.0);
	fixdPos = actualPosMatrix * vec4(in_Position, 1.0);
	exTexCoord = inTexCoord;
	pixelPos = in_Position;
}
