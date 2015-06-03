#include "Debug.h"


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
