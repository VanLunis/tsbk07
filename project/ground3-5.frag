#version 150

in vec2 exTexCoord;
in vec3 frag_Color;
in vec3 exNormal;

out vec3 exTexCoord3;
out vec4 out_Color;
out vec3 out_Normal;

uniform sampler2D textUnit;

void main(void)
{
	//exTexCoord3 = vec3(exTexCoord,1.0);
	vec2 tempTexCoord = vec2(50*exTexCoord.s, 50*exTexCoord.t);
	out_Color = texture(textUnit, tempTexCoord);
	out_Normal = exNormal;
}
