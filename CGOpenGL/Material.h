#pragma once

#include "StandardIncludes.h"
#include "GLBuffer.h"
#include "BufferStructs.h"

class Material
{
public:
	Material();
	Material(glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, float shininess);
	~Material();

	void Bind( const std::unordered_map<std::string, uint32_t>& bindSlots );

private:
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	float shininess;

	GLBuffer<ShaderMaterial_Std140>* materialBuffer;

};

