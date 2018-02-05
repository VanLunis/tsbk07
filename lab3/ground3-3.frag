#version 150

in vec2 exTexCoord;


out vec3 exTexCoord3;
out vec4 out_Color;

uniform sampler2D textUnit;

void main(void)
{
	//exTexCoord3 = vec3(exTexCoord,1.0);
	out_Color = texture(textUnit, exTexCoord);
}
