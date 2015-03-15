#pragma once

#include "VertexFormatManager.h"

#include "StandardIncludes.h"
#include <unordered_map>

class Mesh
{
public:
	Mesh();
	~Mesh();

	int Initialize(VertexFormat format, void* data, uint32_t datasize);

};

