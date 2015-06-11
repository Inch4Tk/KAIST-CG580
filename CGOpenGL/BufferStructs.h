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
	float pad0;
	glm::vec3 worldUp;
	float pad1;
	glm::vec3 worldCamPos;
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