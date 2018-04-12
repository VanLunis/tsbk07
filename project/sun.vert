#version 150
in vec3 in_Position;
in vec2 inTexCoord;
in mat4 transformationMatrix;

uniform mat4 projectionMatrix;
uniform mat4 lookAtMat;




void main(void)
{

gl_Position = projectionMatrix * lookAtMat * transformationMatrix * vec4(in_Position, 1);


}
