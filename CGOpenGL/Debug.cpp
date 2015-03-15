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
	if( msg.empty() )
		return;
	if( type == LogType::None )
		printf( "%s\n", msg );
	else if( type == LogType::Info )
		printf( "Info: %s\n", msg );
	else if( type == LogType::Warning )
		printf( "Warning: %s\n", msg );
	else if( type == LogType::Error )
		printf( "Error: %s\n", msg );
}
