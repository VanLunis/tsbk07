#version 150

uniform int planetNumber;

out vec4 out_Color;

void main()
{
    out_Color = vec4(planetNumber/252.0, 0, 0, 0.0);
}
