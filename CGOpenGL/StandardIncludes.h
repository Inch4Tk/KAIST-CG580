#pragma once

// Include our config defines
#include "Config.h"

// OpenGL libraries
//Include GLEW  
#include <GL/glew.h>

//Include GLFW  
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Include the standard C++ headers  
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <algorithm>
#include <cctype>
#include <functional>
#include <list>
#include <locale>
#include <string>
#include <unordered_map>
#include <vector>

// Extend string by a trim function
// trim from start
static inline std::string &ltrim( std::string &s )
{
	s.erase( s.begin(), std::find_if( s.begin(), s.end(), std::not1( std::ptr_fun<int, int>( std::isspace ) ) ) );
	return s;
}

// trim from end
static inline std::string &rtrim( std::string &s )
{
	s.erase( std::find_if( s.rbegin(), s.rend(), std::not1( std::ptr_fun<int, int>( std::isspace ) ) ).base(), s.end() );
	return s;
}

// trim from both ends
static inline std::string &trim( std::string &s )
{
	return ltrim( rtrim( s ) );
}

// Random number gens (Assume that seedrand has been called)
static inline float randFloat( float min = 0.0f, float max = 1.0f )
{
	return (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (max - min) + min;
}


// Valuable macros
#define SDELETE(p) {if(p){delete p; p = nullptr;}}
#define GLUBYTETOSTR(str) std::string( reinterpret_cast<const char*>(str) )