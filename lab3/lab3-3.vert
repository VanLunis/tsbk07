#version 150

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;
in vec2 inTexCoord;

uniform mat4 translate;
uniform mat4 scale;
uniform mat4 rotateZ;
uniform mat4 rotateY;
uniform mat4 rotateX;
uniform mat4 projectionMatrix;
uniform mat4 mdlMatrix;
uniform mat4 lookAtMat;
uniform mat4 rotationMatrix;
uniform mat4 translationMatrix;
uniform mat4 transformationMatrix;

out vec3 frag_Color;
out vec2 exTexCoord;
out vec3 exNormal;
out vec3 surf;

void main(void)
{
	//exNormal = in_Normal;
	surf = in_Position; //For specular
	//gl_Position = projectionMatrix * lookAtMat * rotateX * rotateY * rotateZ * vec4(in_Position, 1.0);
	gl_Position = projectionMatrix * lookAtMat * transformationMatrix * vec4(in_Position, 1.0);
	mat3 normallookAtMat = mat3(lookAtMat);
	//mat3 normalrotX = mat3(rotateX);
	//mat3 normalrotY = mat3(rotateY);
	//mat3 normalrotZ = mat3(rotateZ);
	mat3 normalRotMat = mat3(transformationMatrix);
	exNormal = normallookAtMat * normalRotMat * in_Normal;
	//gl_Position = vec4(in_Position, 1.0);
	//frag_Color = vec3(shade);
	//exTexCoord = inTexCoord;
}
