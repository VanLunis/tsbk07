#version 150

uniform int planetNumber;

out vec4 out_Color;

void main()
{
    out_Color = vec4(planetNumber/255, 0, 0, 0.0)
}