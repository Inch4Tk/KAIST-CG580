#include "Debug.h"

#include <sstream>

std::unordered_map<GLenum, std::string> Debug::glErrorCodes = {
	{ GL_INVALID_ENUM, "0x0500: GL_INVALID_ENUM" },
	{ GL_INVALID_VALUE, "0x0501: GL_INVALID_VALUE" },
	{ GL_INVALID_OPERATION, "0x0502: GL_INVALID_OPERATION" },
	{ GL_STACK_OVERFLOW, "0x0503: GL_STACK_OVERFLOW" },
	{ GL_STACK_UNDERFLOW, "0x0504: GL_STACK_UNDERFLOW" },
	{ GL_OUT_OF_MEMORY, "0x0505: GL_OUT_OF_MEMORY" },
	{ GL_INVALID_FRAMEBUFFER_OPERATION, "0x0506: GL_INVALID_FRAMEBUFFER_OPERATION" } };

Debug::Debug()
{
}


Debug::~Debug()
{
}

/// <summary>
/// Logs the specified MSG.
/// </summary>
/// <param name="msg">The MSG.</param>
/// <param name="type">The type.</param>
void Debug::Log( const std::string& msg, LogType::LogType type )
{
	if( msg.empty() || msg[0] == '\0' )
		return;
	if( type == LogType::None )
		printf( "%s\n", msg.c_str() );
	else if( type == LogType::Info )
		printf( "Info: %s\n", msg.c_str() );
	else if( type == LogType::Warning )
		printf( "Warning: %s\n", msg.c_str() );
	else if( type == LogType::Error )
		printf( "Error: %s\n", msg.c_str() );
}

void Debug::Log( int i, LogType::LogType type /*= LogType::None */ )
{
	Log( std::to_string( i ), type );
}

void Debug::Log( uint32_t i, LogType::LogType type /*= LogType::None */ )
{
	Log( std::to_string( i ), type );
}

void Debug::Log( uint64_t i, LogType::LogType type /*= LogType::None */ )
{
	Log( std::to_string( i ), type );
}

void Debug::Log( float f, LogType::LogType type /*= LogType::None */ )
{
	Log( std::to_string( f ), type );
}

void Debug::Log( double d, LogType::LogType type /*= LogType::None */ )
{
	Log( std::to_string( d ), type );
}

void Debug::Log( glm::vec2 v, LogType::LogType type /*= LogType::None */ )
{
	Log( std::to_string( v.x ) + ", " + std::to_string( v.y ), type );
}

void Debug::Log( glm::vec3 v, LogType::LogType type /*= LogType::None */ )
{
	Log( std::to_string( v.x ) + ", " + std::to_string( v.y ) + ", " + std::to_string( v.z ), type );
}

void Debug::Log( glm::vec4 v, LogType::LogType type /*= LogType::None */ )
{
	Log( std::to_string( v.x ) + ", " + std::to_string( v.y ) + ", " + std::to_string( v.z ) + ", " + std::to_string( v.w ), type );
}

bool Debug::CheckGLError( const char *file, int line )
{
	bool wasError = false;

	std::stringstream ss;

	for( GLenum glErr = glGetError(); glErr != GL_NO_ERROR; glErr = glGetError() )
	{
		wasError = true;

		/*const GLubyte* sError = glewGetErrorString( glErr );
		if( !sError )
		{
		sError = reinterpret_cast<const GLubyte*>("(no message available)");
		}*/
		auto errEntry = glErrorCodes.find( glErr );
		std::string sError = "(no message available)";
		if( errEntry != glErrorCodes.end() )
		{
			sError = errEntry->second;
		}
		
		ss << "  GL Error #" << sError << std::endl;
	}

	if( wasError )
	{
		Debug::LogFailure( file, line, ss.str() );
	}
	return wasError;
}

void Debug::LogFailure( const char *file, const int line, const char* error )
{
	std::stringstream errorstream;
	errorstream << file << "(" << line << "): " << error;
	Debug::Log( errorstream.str(), LogType::Error );
}

void Debug::LogFailure( const char *file, const int line, std::string& error )
{
	std::stringstream errorstream;
	errorstream << file << "(" << line << "): " << error;
	Debug::Log( errorstream.str(), LogType::Error );
}
