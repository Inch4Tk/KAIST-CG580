#pragma once

#include "VertexFormatManager.h"

#include "StandardIncludes.h"
#include <unordered_map>

// Forwards
class Material;

class Mesh
{
public:
	Mesh();
	~Mesh();

	int Initialize( const VertexFormat& format, void* data, uint32_t datasize,
					GLenum primitiveType, Material* material = nullptr );

	int Initialize( const VertexFormat& format, void* vdata, uint32_t vdatasize, const std::vector<uint32_t>& indices,
					GLenum primitiveType, Material* material = nullptr );

	const void Draw();

private:
	bool initialized = false;

	Material* mat;

	GLuint vaoID = 0;
	GLuint vboID = 0;

	VertexFormat vbFormat;

	GLenum primitiveType;
	uint32_t numVertices;
	uint32_t numIndices;
};

