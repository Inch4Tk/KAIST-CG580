#pragma once

#include "StandardIncludes.h"

struct BindSlots
{
	static std::unordered_map<std::string, uint32_t> defaultBindSlots;
};

struct ShaderGlobals_Std140
{
	glm::mat4 worldMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 viewProjectionMatrix;
	glm::vec3 ambient;
	float invLogSubDiv; // 1.0f / log(subDivision + 1.0f); subDivision = 2.0f * tan( fov * 0.5f ) / gridDimY;
	glm::vec3 worldUp;
	float invNear; // 1.0f / nearPlane
	glm::vec3 worldCamPos;
	uint32_t dimTilesX;
	uint32_t dimTilesY;
	uint32_t amtTilesX;
	uint32_t amtTilesY;
};

struct ShaderMaterial_Std140
{
	glm::vec3 diffuse;
	float pad0;
	glm::vec3 specular;
	float pad1;
	glm::vec3 ambient;
	float shininess;
};

struct ShaderLight_Std140
{
	glm::vec3 position;
	float pad0;
	glm::vec3 color;
	float range;
	ShaderLight_Std140()
	{
	};
	ShaderLight_Std140( glm::vec3 position, glm::vec3 color, float range ) 
		: position( position ), color( color ), range( range )
	{
	};
};