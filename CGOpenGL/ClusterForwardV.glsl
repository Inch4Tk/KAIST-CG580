#version 430 core

layout( std140 ) uniform UniGlobals
{
	mat4 worldMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
	vec3 ambient;
	vec3 worldUp;
	vec3 worldCamPos;
	float invNear; // 1.0f / nearPlane
	float invLogSubDiv; // 1.0f / log(subDivision + 1.0f); subDivision = 2.0f * tan( fov * 0.5f ) / gridDimY;
};

layout( location = 0 ) in vec3 vPos;
layout( location = 1 ) in vec3 vNorm;

out VertexData{
	vec3 worldPos;
	vec3 viewPos;
	vec3 normal;
} VertexOut;

void main()
{
	VertexOut.normal = vNorm;
	VertexOut.worldPos = (worldMatrix * vec4( vPos, 1 )).xyz;
	VertexOut.viewPos = (viewMatrix * vec4( VertexOut.worldPos, 1 )).xyz;
	gl_Position = projectionMatrix * vec4( VertexOut.viewPos, 1 );
}