#pragma once

#include "StandardIncludes.h"

#ifdef _DEBUG
#define CHECK_GL_ERROR() { Debug::CheckGLError(__FILE__, __LINE__) && (__debugbreak(), 1); }
#define ASSERT(_condition_) \
  if (!(_condition_)) \
          { \
    Debug::LogFailure(__FILE__, __LINE__, #_condition_); \
    __debugbreak(); \
          }
#else
#define CHECK_GL_ERROR() {  }
#define ASSERT(_condition_)
#endif

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

	// Quick and dirty default failure log type log
	static void LogFailure( const char *file, const int line, std::string& error);
	// Use macro over this, because of the debugbreak
	static bool CheckGLError( const char *file, int line );
private:
	Debug();
	~Debug();

};

