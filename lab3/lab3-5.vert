#version 150

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;
in vec2 inTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 lookAtMat;
uniform mat4 transformationMatrix;


uniform vec3 lightSourcesDirPosArr[4];
uniform vec3 lightSourcesColorArr[4];
uniform float specularExponent;
uniform bool isDirectional[4];

out vec3 frag_Color;
out vec2 exTexCoord;
out vec3 exNormal;
out vec3 surf;
out vec3 worldNormal;

void main(void)
{
	//surf = in_Position; //For specular
	surf = (transformationMatrix * vec4(in_Position, 1.0)).xyz;
	gl_Position = projectionMatrix * lookAtMat * transformationMatrix * vec4(in_Position, 1.0);
	//mat3 normallookAtMat = mat3(lookAtMat);
	//mat3 normalRotMat = mat3(transformationMatrix);
	//exNormal = normallookAtMat * normalRotMat * in_Normal;
	worldNormal = normalize(mat3(transformationMatrix) * in_Normal);
}
