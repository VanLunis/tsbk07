#version 150
in vec3 frag_Color;
in vec2 exTexCoord;

out vec4 out_Color;
out vec4 out_Normal;
out vec3 exTexCoord3;

uniform float t;
uniform sampler2D textUnit;

void main(void)
{
	exTexCoord3 = vec3(exTexCoord,1.0);
	float a = sin(exTexCoord3.s*30 * (1.0+sin(t/2.0)))/2 + 0.5;
	float b = sin(exTexCoord.t * 30.0 * (1.0+sin(t/4.0)))/2.0 + 0.5;
	//out_Color = vec4(a, b, 1.0, 0.0);
	out_Color = texture(textUnit, exTexCoord);

}
