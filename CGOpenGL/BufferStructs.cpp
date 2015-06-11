#include "BufferStructs.h"

std::unordered_map<std::string, uint32_t> BindSlots::defaultBindSlots = {
	// Uniforms
	{ "UniGlobals", 0 },
	{ "UniMaterial", 1 },
	{ "UniLights", 2 }
};