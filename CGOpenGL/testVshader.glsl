#version 430 core

layout( std140 ) uniform UniGlobals
{
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
	vec3 ambient;
	float invLogSubDiv; // 1.0f / log(subDivision + 1.0f); subDivision = 2.0f * tan( fov * 0.5f ) / gridDimY;
	vec3 worldUp;
	float invNear; // 1.0f / nearPlane
	vec3 worldCamPos;
	uint dimTilesX;
	uint dimTilesY;
	uint amtTilesX;
	uint amtTilesY;
};
uniform mat4 worldMatrixNormal;
uniform mat4 worldMatrix;

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;

out VertexData {
    vec3 normal;
} VertexOut;

void main()
{   
	VertexOut.normal = (worldMatrixNormal * vec4(vNorm, 0)).xyz;
	gl_Position = viewProjectionMatrix * worldMatrix * vec4(vPos,1);
 }