#version 150
in vec3 frag_Color;
out vec4 out_Color;

void main(void)
{
	out_Color = vec4(frag_Color,1.0);
}
