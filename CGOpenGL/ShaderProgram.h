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

	static const ShaderProgram* LoadProgram( const std::string& programName, const InitConfig& config,
											 const std::unordered_map<std::string, uint32_t>& uniSlots );

	GLint GetUniformLocation( const std::string& name ) const;
	void BindShader() const;
	const std::unordered_map<std::string, uint32_t>& GetUniBufferSlots();
private:
	GLuint LoadShader( const std::string& filename, GLenum type );

	bool initialized = false;
	InitConfig config;
	GLuint programID = 0;
	std::unordered_map<std::string, uint32_t> uniSlots;
};

