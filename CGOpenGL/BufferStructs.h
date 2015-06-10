#pragma once

#include "StandardIncludes.h"

struct BufferSlots
{
	static std::unordered_map<std::string, uint32_t> defaultUniformSlots;
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