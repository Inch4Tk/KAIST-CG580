#pragma once

#include "StandardIncludes.h"

#include <unordered_map>

struct VertexFormat
{
	uint32_t bytesize;
	std::vector<GLint> sizes;
	std::vector<GLenum> types;
	std::vector<uint32_t> offsets;
	std::vector<bool> asInt; // Do not cast to a float but use glVertexAttribIPointer 
	std::vector<bool> normalized;
};

class VertexFormatManager
{
public:
	static const VertexFormat& Get3F();
	static const VertexFormat& Get3F3F();
	static const VertexFormat& Get3F3F2F();
private:
	VertexFormatManager();
	~VertexFormatManager();

	static std::unordered_map<std::string, VertexFormat> formats;
};