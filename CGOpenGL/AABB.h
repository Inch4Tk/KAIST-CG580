#pragma once

#include "StandardIncludes.h"

class AABB
{
public:
	AABB();
	AABB( glm::vec3 positionBLF, glm::vec3 extents );
	~AABB();

	glm::vec3 positionBLF; // Bottom Left Front, by definition always minimum, z is flipped
	glm::vec3 extents;
	glm::vec3 maxExtents; // convenience
};

