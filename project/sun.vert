#version 150

in vec3 in_Position;
in vec3 in_Color;
in vec3 in_Normal;
in vec2 inTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 lookAtMat;
uniform mat4 transformationMatrix;
uniform mat4 actualPosMatrix;

uniform vec3 SunPos;
uniform vec3 lightSourcesColorArr;
uniform float specularExponent;
uniform bool isDirectional;


uniform sampler2D dirtTex;
uniform sampler2D rutorTex;

out vec3 frag_Color;
out vec2 exTexCoord;
out vec3 exNormal;
out vec3 surf;
out vec3 worldNormal;
out vec3 pixelPos;

out vec4 fixdPos;

void main(void)
{
	//exNormal = in_Normal;
	//surf = in_Position; //For specular
	surf = (transformationMatrix * vec4(in_Position, 1.0)).xyz;
	worldNormal = normalize(mat3(transformationMatrix) * in_Normal);
	//gl_Position = projectionMatrix * lookAtMat * rotateX * rotateY * rotateZ * vec4(in_Position, 1.0);
	gl_Position = projectionMatrix * lookAtMat * transformationMatrix * vec4(in_Position, 1.0);
	fixdPos = actualPosMatrix * vec4(in_Position, 1.0);
	//mat3 normallookAtMat = mat3(lookAtMat);
	//mat3 normalrotX = mat3(rotateX);
	//mat3 normalrotY = mat3(rotateY);
	//mat3 normalrotZ = mat3(rotateZ);
	//mat3 normalRotMat = mat3(transformationMatrix);
	//exNormal = normallookAtMat * normalRotMat * in_Normal;
	//gl_Position = vec4(in_Position, 1.0);
	//frag_Color = vec3(shade);
	exTexCoord = inTexCoord;
	pixelPos = in_Position;
}
