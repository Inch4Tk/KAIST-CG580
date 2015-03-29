#include "ShaderProgram.h"

#include "Debug.h"

#include <algorithm>
#include <fstream>

ShaderProgram::ShaderProgram()
{
}


ShaderProgram::~ShaderProgram()
{
	glDeleteProgram( programID );
}

/// <summary>
/// Loads the shader.
/// </summary>
/// <param name="filename">The filename.</param>
/// <param name="type">The type.</param>
/// <returns></returns>
GLuint ShaderProgram::LoadShader( const std::string& filename, GLenum type )
{
	// Create the shader
	GLuint shaderID = glCreateShader( type );

	// Read the Shader code from the file
	std::string shaderCode;
	std::ifstream shaderStream( filename, std::ios::in );
	if( shaderStream.is_open() )
	{
		std::string line = "";
		while( getline( shaderStream, line ) )
			shaderCode += "\n" + line;
		shaderStream.close();
	}

	GLint result = GL_FALSE;
	int infoLogLength;

	// Compile Shader
	Debug::Log( "Compiling shader: " + filename );
	char const * sourcePointer = shaderCode.c_str();
	glShaderSource( shaderID, 1, &sourcePointer, NULL );
	glCompileShader( shaderID );

	// Check Shader
	glGetShaderiv( shaderID, GL_COMPILE_STATUS, &result );
	glGetShaderiv( shaderID, GL_INFO_LOG_LENGTH, &infoLogLength );
	std::vector<char> shaderErrorMessage( infoLogLength );
	glGetShaderInfoLog( shaderID, infoLogLength, NULL, &shaderErrorMessage[0] );
	Debug::Log( std::string( shaderErrorMessage.begin(), shaderErrorMessage.end() ), LogType::Error );

	return shaderID;
}

/// <summary>
/// Loads the program, empty strings in the config are not loaded or linked shader stages. 
/// There are no errors for incomplete programs.
/// </summary>
/// <param name="config">The configuration.</param>
/// <returns></returns>
int ShaderProgram::LoadProgram( const InitConfig& config )
{
	// Create the program
	programID = glCreateProgram();

	// Load and attach all the shaders
	GLuint vshader = 0;
	GLuint tcshader = 0;
	GLuint teshader = 0;
	GLuint gshader = 0;
	GLuint fshader = 0;
	GLuint cshader = 0;
	if( !config.vsPath.empty() )
	{
		vshader = LoadShader( config.vsPath, GL_VERTEX_SHADER );
		glAttachShader( programID, vshader );
	}
	if( !config.tcsPath.empty() )
	{
		tcshader = LoadShader( config.vsPath, GL_TESS_CONTROL_SHADER );
		glAttachShader( programID, tcshader );
	}
	if( !config.tesPath.empty() )
	{
		teshader = LoadShader( config.vsPath, GL_TESS_EVALUATION_SHADER );
		glAttachShader( programID, teshader );
	}
	if( !config.gsPath.empty() )
	{
		gshader = LoadShader( config.vsPath, GL_GEOMETRY_SHADER );
		glAttachShader( programID, gshader );
	}
	if( !config.fsPath.empty() )
	{
		fshader = LoadShader( config.vsPath, GL_FRAGMENT_SHADER );
		glAttachShader( programID, fshader );
	}
	if( !config.csPath.empty() )
	{
		cshader = LoadShader( config.vsPath, GL_COMPUTE_SHADER );
		glAttachShader( programID, cshader );
	}

	// Link program
	glLinkProgram( programID );

	// Check the program
	GLint result = GL_FALSE;
	int infoLogLength;
	glGetProgramiv( programID, GL_LINK_STATUS, &result );
	glGetProgramiv( programID, GL_INFO_LOG_LENGTH, &infoLogLength );
	std::vector<char> programErrorMessage( std::max( infoLogLength, int( 1 ) ) );
	glGetProgramInfoLog( programID, infoLogLength, NULL, &programErrorMessage[0] );
	Debug::Log( std::string( programErrorMessage.begin(), programErrorMessage.end() ), LogType::Error );
	
	// Cleanup loaded shader
	glDeleteShader( vshader );
	glDeleteShader( tcshader );
	glDeleteShader( teshader );
	glDeleteShader( gshader );
	glDeleteShader( fshader );
	glDeleteShader( cshader );

	return 0;
}

/// <summary>
/// Binds the shader.
/// </summary>
/// <returns></returns>
const void ShaderProgram::BindShader()
{
	if( !initialized )
		return;

	glUseProgram( programID );
}
