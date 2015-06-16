#pragma once

#include "StandardIncludes.h"

class AABB
{
public:
	AABB();
	AABB( glm::vec3 positionBLB, glm::vec3 extents );
	~AABB();

	glm::vec3 positionBLB; // Bottom Left Back, by definition always minimum
	glm::vec3 extents;
	glm::vec3 maxExtents; // convenience
};

