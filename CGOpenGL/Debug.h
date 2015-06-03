#pragma once

#include "StandardIncludes.h"

namespace LogType
{
	enum LogType
	{
		None = 0,
		Info = (1 << 0),
		Warning = (1 << 1),
		Error = (1 << 2),
	};
}

class Debug
{
public:

	static void Log( const std::string& msg, LogType::LogType type = LogType::None );

	static void Log( int i, LogType::LogType type = LogType::None );
	static void Log( uint32_t i, LogType::LogType type = LogType::None );
	static void Log( uint64_t i, LogType::LogType type = LogType::None );
	static void Log( float f, LogType::LogType type = LogType::None );
	static void Log( double d, LogType::LogType type = LogType::None );
	static void Log( glm::vec2 v, LogType::LogType type = LogType::None );
	static void Log( glm::vec3 v, LogType::LogType type = LogType::None );
	static void Log( glm::vec4 v, LogType::LogType type = LogType::None );


private:
	Debug();
	~Debug();

};

