#pragma once

// Include the standard C++ headers  
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <list>
#include <string>
#include <vector>
#include <unordered_map>

// Valuable macros
#define SDELETE(p) {if(p){delete p; p = nullptr;}}
#define GLUBYTETOSTR(str) std::string( reinterpret_cast<const char*>(str) )