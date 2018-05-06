#version 150

in vec3 in_Position;

uniform mat4 projectionMatrix;
uniform mat4 lookAtMat;
uniform mat4 transformationMatrix;
uniform int planetNumber;

void main(void)
{
	gl_Position = projectionMatrix * lookAtMat * transformationMatrix * vec4(in_Position, 1.0);
}
