#include "ShaderProgram.h"

#include "AppManager.h"
#include "Debug.h"
#include "ObjectManager.h"
#include "Scene.h"

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
	else
	{
		Debug::Log( "Could not find shader with path: " + filename );
		return 0;
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
	Debug::Log( trim( std::string( shaderErrorMessage.begin(), shaderErrorMessage.end() ) ), LogType::Error );

	CHECK_GL_ERROR();
	return shaderID;
}

/// <summary>
/// Loads the program, empty strings in the config are not loaded or linked shader stages. 
/// There are no errors for incomplete programs.
/// </summary>
/// <param name="programName">Name of the program.</param>
/// <param name="config">The configuration.</param>
/// <param name="uniSlots">The uniform buffer slots.</param>
/// <returns></returns>
const ShaderProgram* ShaderProgram::LoadProgram( const std::string& programName, const InitConfig& config,
												 const std::unordered_map<std::string, uint32_t>& uniSlots )
{
	// Check if the shader already exists
	ShaderProgram* sp = AppManager::GetScene()->GetShader( programName );
	if( sp != nullptr)
		return sp;

	// Create the program
	sp = new ShaderProgram();
	sp->config = config;
	sp->programID = glCreateProgram();

	// Load and attach all the shaders
	GLuint vshader = 0;
	GLuint tcshader = 0;
	GLuint teshader = 0;
	GLuint gshader = 0;
	GLuint fshader = 0;
	GLuint cshader = 0;
	if( !config.vsPath.empty() )
	{
		vshader = sp->LoadShader( config.vsPath, GL_VERTEX_SHADER );
		glAttachShader( sp->programID, vshader );
	}
	if( !config.tcsPath.empty() )
	{
		tcshader = sp->LoadShader( config.tcsPath, GL_TESS_CONTROL_SHADER );
		glAttachShader( sp->programID, tcshader );
	}
	if( !config.tesPath.empty() )
	{
		teshader = sp->LoadShader( config.tesPath, GL_TESS_EVALUATION_SHADER );
		glAttachShader( sp->programID, teshader );
	}
	if( !config.gsPath.empty() )
	{
		gshader = sp->LoadShader( config.gsPath, GL_GEOMETRY_SHADER );
		glAttachShader( sp->programID, gshader );
	}
	if( !config.fsPath.empty() )
	{
		fshader = sp->LoadShader( config.fsPath, GL_FRAGMENT_SHADER );
		glAttachShader( sp->programID, fshader );
	}
	if( !config.csPath.empty() )
	{
		cshader = sp->LoadShader( config.csPath, GL_COMPUTE_SHADER );
		glAttachShader( sp->programID, cshader );
	}

	// Link program
	glLinkProgram( sp->programID );

	// Check the program
	GLint result = GL_FALSE;
	int infoLogLength;
	glGetProgramiv( sp->programID, GL_LINK_STATUS, &result );
	glGetProgramiv( sp->programID, GL_INFO_LOG_LENGTH, &infoLogLength );
	std::vector<char> programErrorMessage( std::max( infoLogLength, int( 1 ) ) );
	glGetProgramInfoLog( sp->programID, infoLogLength, NULL, &programErrorMessage[0] );
	Debug::Log( trim( std::string( programErrorMessage.begin(), programErrorMessage.end() ) ), LogType::Error );
	
	// Cleanup loaded shader
	glDeleteShader( vshader );
	glDeleteShader( tcshader );
	glDeleteShader( teshader );
	glDeleteShader( gshader );
	glDeleteShader( fshader );
	glDeleteShader( cshader );

	sp->initialized = true;

	// Bind the uniform buffer block slots
	for( auto it = uniSlots.begin(); it != uniSlots.end(); ++it )
	{
		int loc = glGetUniformBlockIndex( sp->programID, it->first.c_str() );
		CHECK_GL_ERROR();
		if( loc >= 0 )
		{
			glUniformBlockBinding( sp->programID, loc, it->second );
			CHECK_GL_ERROR();
			// Build our list of actually existing uni buffers, so we don't bind something not allowed later
			sp->bindSlots.emplace(std::pair<std::string, uint32_t>(it->first, it->second));
		}
	}

	// Register with scene
	AppManager::GetScene()->RegisterShader( programName, sp );
	CHECK_GL_ERROR();
	return sp;
}

/// <summary>
/// Binds the shader.
/// </summary>
/// <returns></returns>
void ShaderProgram::BindShader() const
{
	if( !initialized )
		return;

	// Bind shader program
	glUseProgram( programID );
	CHECK_GL_ERROR();
	// Bind the per frame buffers
	AppManager::GetObjectManager()->BindPerFrameUniformBuffer( bindSlots );
}

/// <summary>
/// Gets the uniform location.
/// </summary>
/// <param name="name">The name.</param>
/// <returns></returns>
GLint ShaderProgram::GetUniformLocation( const std::string& name ) const
{
	return glGetUniformLocation( programID, name.c_str() );
}

/// <summary>
/// Gets the uni buffer slots.
/// </summary>
/// <returns></returns>
const std::unordered_map<std::string, uint32_t>& ShaderProgram::GetBindSlots() const
{
	return bindSlots;
}
