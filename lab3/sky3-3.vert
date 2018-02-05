#version 150

in vec3 in_Position;
in vec2 inTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 lookAtMat;
uniform mat4 transformationMatrix;

out vec2 exTexCoord;

void main(void)
{
	exTexCoord = inTexCoord;
	gl_Position = projectionMatrix * transformationMatrix * vec4(in_Position, 1.0);
	//gl_Position = projectionMatrix * vec4(in_Position, 1.0);
}
