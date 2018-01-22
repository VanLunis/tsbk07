#version 150
in vec3 frag_Color;

out vec4 out_Color;
out vec4 out_Normal;

void main(void)
{
	//out_Color = 	out_Color = vec4(0.4, 0.0, 0.8, 1.0);
	out_Color = 	out_Color = vec4(frag_Color, 1.0);

}
