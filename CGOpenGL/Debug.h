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

private:
	Debug();
	~Debug();

};

