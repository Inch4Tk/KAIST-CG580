#pragma once

#include <cstdint>

#ifndef CONFIG_MAX_LIGHTS
#define CONFIG_MAX_LIGHTS 1024
#endif

#ifndef ENABLE_CUDA
#define ENABLE_CUDA false
#endif

class Config
{
public:
	static uint32_t DIM_TILES_X; // Set in App Manager
	static uint32_t DIM_TILES_Y; // Set in App Manager
	static uint32_t AMT_TILES_X; // Updated on window size setting
	static uint32_t AMT_TILES_Y; // Updated on window size setting
	static uint32_t AMT_TILES_Z; // Updated based on camera
	static float FOV_Y; // Used to init camera, set in App Manager, not a nice solution but meh
	static float ASPECT; // Same
	static float NEAR_PLANE; // Same
	static float FAR_PLANE; // Same
};
