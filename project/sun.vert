#version 150
in vec3 in_Position;
//in vec3 in_Normal;
in vec2 inTexCoord;
in mat4 transformationMatrix;

uniform mat4 projectionMatrix;
uniform mat4 lookAtMat;

//out vec3 ex_Normal;


void main(void)
{

gl_Position = projectionMatrix * lookAtMat * transformationMatrix * vec4(in_Position, 1);

//ex_Normal = in_Normal;
}
