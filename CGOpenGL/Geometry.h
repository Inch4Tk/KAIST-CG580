#pragma once

#include "StandardIncludes.h"

// Forwards
class Mesh;

// Basically a wrapper class around multiple meshes to make it possible for a scene object to contain more meshes
// and to allow for easy obj file loading
class Geometry
{
public:
	Geometry( const std::string& objFilename );
	Geometry( const std::vector<Mesh*>& meshes );
	~Geometry();

	void Draw( const std::unordered_map<std::string, uint32_t>& bindSlots ) const;

private:
	std::vector<Mesh*> meshes;
};

