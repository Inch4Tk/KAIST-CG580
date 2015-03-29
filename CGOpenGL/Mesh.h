#pragma once

#include "VertexFormatManager.h"

#include "StandardIncludes.h"
#include <unordered_map>

class Mesh
{
public:
	Mesh();
	~Mesh();

	int Initialize( const VertexFormat& format, void* data, uint32_t datasize,
					GLenum primitiveType );

	const void Draw();

private:
	bool initialized = false;

	GLuint vaoID = 0;
	GLuint vboID = 0;

	VertexFormat vbFormat;

	GLenum primitiveType;
	uint32_t numVertices;
	uint32_t numIndices;
};

