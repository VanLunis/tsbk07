#version 150

in vec3 in_Position;
in vec3 in_Color;
uniform mat4 translate;
uniform mat4 scale;
uniform mat4 rotateZ;
uniform mat4 rotateY;
uniform mat4 rotateX;
out vec3 frag_Color;

void main(void)
{
	  gl_Position =rotateZ * rotateY * rotateX * vec4(in_Position, 1.0);
		frag_Color = in_Color;
}
