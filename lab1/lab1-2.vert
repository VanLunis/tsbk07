#version 150

in  vec3 in_Position;
uniform mat4 translate;
uniform mat4 scale;
uniform mat4 rotate;

void main(void)
{
	  gl_Position = translate * scale * rotate * vec4(in_Position, 1.0);
}
