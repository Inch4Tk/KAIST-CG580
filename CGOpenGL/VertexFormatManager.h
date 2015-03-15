#pragma once

#include "StandardIncludes.h"

#include "GLM/glm.hpp"
#include "GL/glew.h"

#include <unordered_map>

struct VertexFormat
{
	std::vector<GLint> sizes;
	std::vector<GLenum> types;
	std::vector<bool> normalized;
};

class VertexFormatManager
{
public:
	static VertexFormat Get3F();
private:
	VertexFormatManager();
	~VertexFormatManager();

	static std::unordered_map<std::string, VertexFormat> formats;
};