#include "AABB.h"


AABB::AABB( glm::vec3 positionBLF, glm::vec3 extents ) : positionBLF( positionBLF ), extents( extents )
{
	this->maxExtents = positionBLF + extents;
	// Flip zmin and zmax
	float t = positionBLF.z;
	this->positionBLF.z = maxExtents.z;
	this->maxExtents.z = t;
}

/// <summary>
/// Creates an AABB that covers everything (almost) 1000*farplane, any interactions that should be bigger than that... handle them differently
/// </summary>
AABB::AABB()
{
	positionBLF = glm::vec3( -500 * Config::FAR_PLANE );
	extents = glm::vec3( 1000 * Config::FAR_PLANE );
	maxExtents = positionBLF + extents;
	// Flip zmin and zmax
	float t = positionBLF.z;
	positionBLF.z = maxExtents.z;
	maxExtents.z = t;
}


AABB::~AABB()
{
}
