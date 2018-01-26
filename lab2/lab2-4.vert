#version 150

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;
in vec2 inTexCoord;

uniform mat4 translate;
uniform mat4 scale;
uniform mat4 rotateZ;
uniform mat4 rotateY;
uniform mat4 rotateX;
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 lookAtMat;

out vec3 frag_Color;
out vec2 exTexCoord;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade;
	shade = dot(normalize(in_Normal), light);
	gl_Position = projectionMatrix * lookAtMat *  rotateX * rotateY * rotateZ * vec4(in_Position, 1.0);
	//gl_Position = vec4(in_Position, 1.0);
	frag_Color = vec3(shade);
	exTexCoord = inTexCoord;
}
