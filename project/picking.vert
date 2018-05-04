#version 150

in vec3 in_Position;

uniform mat4 projectionMatrix;
uniform mat4 lookAtMat;
uniform mat4 transformationMatrix;
uniform mat4 actualPosMatrix;

uniform int planetNumber;


out vec3 surf;
out vec3 worldNormal;
out vec3 pixelPos;
out vec2 exTexCoord;

out vec4 fixdPos;

void main(void)
{
	gl_Position = projectionMatrix * lookAtMat * transformationMatrix * vec4(in_Position, 1.0);
}
