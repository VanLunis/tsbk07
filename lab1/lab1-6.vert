#version 150

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;

uniform mat4 translate;
uniform mat4 scale;
uniform mat4 rotateZ;
uniform mat4 rotateY;
uniform mat4 rotateX;

out vec3 frag_Color;

void main(void)
{
const vec3 light = vec3(0.58, 0.58, 0.58);
float shade;
shade = dot(normalize(in_Normal), light);
	  gl_Position =rotateZ * rotateY * rotateX * vec4(in_Position, 1.0);
			//gl_Position = vec4(in_Position, 1.0);
		frag_Color = vec3(shade);
}
