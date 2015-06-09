#version 430 core

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewProjectionMatrix;

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;

out VertexData {
	vec3 worldPos;
    vec3 normal;
} VertexOut;

void main()
{   
	VertexOut.normal = vNorm;
	VertexOut.worldPos = (worldMatrix * vec4(vPos,1)).xyz;
	gl_Position = viewProjectionMatrix * vec4(VertexOut.worldPos, 1);
 }