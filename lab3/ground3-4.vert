#version 150

in vec3 in_Position;
in vec2 inTexCoord;
in vec3 in_Normal;

uniform mat4 projectionMatrix;
uniform mat4 lookAtMat;
uniform mat4 transformationMatrix;

out vec2 exTexCoord;
out vec3 exNormal;

void main(void)
{
	//const vec3 light = vec3(0.58, 0.58, 0.58);
	//float shade;
	//shade = dot(normalize(in_Normal), light);
	gl_Position = projectionMatrix * lookAtMat * transformationMatrix * vec4(in_Position, 1.0);
	exTexCoord = inTexCoord;
	exNormal = in_Normal;
	//gl_Position = projectionMatrix * vec4(in_Position, 1.0);
	//frag_Color = vec3(shade);
}
