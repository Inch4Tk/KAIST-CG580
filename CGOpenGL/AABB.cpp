#include "AABB.h"


AABB::AABB( glm::vec3 positionBLB, glm::vec3 extents ) : positionBLB( positionBLB ), extents( extents )
{
	maxExtents = positionBLB + extents;
}


AABB::~AABB()
{
}
