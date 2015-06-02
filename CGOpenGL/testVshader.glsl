#version 330 core

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 viewProjectionMatrix;

layout(location = 0) in vec3 vPos;

void main()
{    
	gl_Position = viewProjectionMatrix * worldMatrix * vec4(vPos,1);
 }