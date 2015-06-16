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
uniform mat4 worldMatrix;

const vec3 lightDir = {0.7, 1.0, 0.0};

in VertexData {
    vec3 normal;
} FragmentIn;

out vec4 color;

void main()
{
	vec3 nLightDir = normalize(lightDir);
	float light = clamp(dot(normalize(FragmentIn.normal), nLightDir), 0, 1);
    color = vec4(vec3(1,0,0) * light + vec3(0.1,0.1,0.1), 1);
}