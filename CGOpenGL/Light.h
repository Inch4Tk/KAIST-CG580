#pragma once

#include "StandardIncludes.h"

#include "BufferStructs.h"
#include "SceneObject.h"

class Light :
	public SceneObject
{
public:
	enum LightType // For now only supports point lights
	{
		POINT_LIGHT
	};

	Light(); // Default type point light
	Light( glm::vec3 position, glm::vec3 color, float range ); // Inferred light type: point light
	Light( glm::vec3 position, Light& copyFrom ); // Copies settings from the provided light, but changes position
	~Light();

	// Getters and setters
	bool IsActive() const;
	void Activate( bool val );
	glm::vec3 GetColor() const;
	void SetColor( glm::vec3 val );
	float GetRange() const;
	void SetRange( float val );

private:
	bool active = true;
	LightType lightType = POINT_LIGHT;

	glm::vec3 color = {1,1,1};
	float range = 0.0f;
};

