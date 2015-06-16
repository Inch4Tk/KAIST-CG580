#include "Config.h"

uint32_t Config::DIM_TILES_X; // Set in App Manager
uint32_t Config::DIM_TILES_Y; // Set in App Manager
uint32_t Config::AMT_TILES_X; // Updated on window size setting
uint32_t Config::AMT_TILES_Y; // Updated on window size setting
uint32_t Config::AMT_TILES_Z; // Updated based on camera
float Config::FOV_Y; // Used to init camera, set in App Manager, not a nice solution but meh
float Config::ASPECT; // Same
float Config::NEAR_PLANE; // Same
float Config::FAR_PLANE; // Same