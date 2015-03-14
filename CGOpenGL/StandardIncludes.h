#pragma once

// Include the standard C++ headers  
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <vector>
#include <list>

// Valuable macros
#define SDELETE(p) {if(p){delete p; p = nullptr;}}