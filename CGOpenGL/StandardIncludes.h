#pragma once

// Include the standard C++ headers  
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <list>
#include <string>
#include <vector>

// Valuable macros
#define SDELETE(p) {if(p){delete p; p = nullptr;}}