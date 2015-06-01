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

	static const ShaderProgram* LoadProgram( const std::string& programName, const InitConfig& config );

	void BindShader() const;

private:
	GLuint LoadShader( const std::string& filename, GLenum type );

	bool initialized = false;
	InitConfig config;
	GLuint programID = 0;
};

