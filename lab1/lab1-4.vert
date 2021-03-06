#version 150

in vec3 in_Position;
in vec3 in_Color;
uniform mat4 translate;
uniform mat4 scale;
uniform mat4 rotate;
out vec3 frag_Color;

void main(void)
{
	  gl_Position = translate * rotate * vec4(in_Position, 1.0);
		frag_Color = in_Color;
}
