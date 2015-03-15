#pragma once

#include "GL\glew.h"

#include "StandardIncludes.h"

class ShaderProgram
{
public:
	struct InitConfig
	{
		std::string vsPath = "";
		std::string tcsPath = "";
		std::string tesPath = "";
		std::string gsPath = "";
		std::string fsPath = "";
		std::string csPath = "";
	};

	ShaderProgram();
	~ShaderProgram();

	int LoadProgram( const InitConfig& config );

private:
	GLuint LoadShader( const std::string& filename, GLenum type );

	bool initialized = false;
	InitConfig config;
	GLuint programID;
};

