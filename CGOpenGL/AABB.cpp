#include "AABB.h"


AABB::AABB( glm::vec3 positionBLB, glm::vec3 extents ) : positionBLB( positionBLB ), extents( extents )
{
	maxExtents = positionBLB + extents;
	// Flip zmin and zmax
	float t = positionBLB.z;
	positionBLB.z = maxExtents.z;
	maxExtents.z = t;
}

/// <summary>
/// Creates an AABB that covers everything (almost)
/// </summary>
AABB::AABB()
{
	positionBLB = glm::vec3( 0.01f * -FLT_MAX );
	extents = glm::vec3( 0.02f * FLT_MAX );
	maxExtents = positionBLB + extents;
	// Flip zmin and zmax
	float t = positionBLB.z;
	positionBLB.z = maxExtents.z;
	maxExtents.z = t;
}


AABB::~AABB()
{
}
