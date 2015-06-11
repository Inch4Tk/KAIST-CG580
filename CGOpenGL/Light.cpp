#include "Light.h"

#include "AppManager.h"
#include "Debug.h"
#include "Geometry.h"
#include "ObjectManager.h"
#include "ShaderProgram.h"


Light::Light()
{
}

/// <summary>
/// Initializes a new instance of the <see cref="Light"/> class. Inferred light type: point light.
/// </summary>
/// <param name="position">The position.</param>
/// <param name="color">The color.</param>
/// <param name="range">The range.</param>
Light::Light( glm::vec3 position, glm::vec3 color, float range ) : color(color), range(range)
{
	this->position = position;
}

/// <summary>
/// Initializes a new instance of the <see cref="Light"/> class. Copies settings from the provided light, but changes position.
/// </summary>
/// <param name="position">The position.</param>
/// <param name="copyFrom">The copy from.</param>
Light::Light( glm::vec3 position, Light& copyFrom )
{
	this->position = position;

	// Copy all the other attributes
	active = copyFrom.active;
	lightType = copyFrom.lightType;
	color = copyFrom.color;
	range = copyFrom.range;
}


Light::~Light()
{
	AppManager::GetObjectManager()->RemoveLight( this );
}


/// <summary>
/// Determines whether the light is active.
/// </summary>
/// <returns></returns>
bool Light::IsActive() const
{
	return active;
}
/// <summary>
/// Activates this instance.
/// </summary>
/// <param name="val">if set to <c>true</c> [value].</param>
void Light::Activate( bool val )
{
	active = val;
}
/// <summary>
/// Gets the color.
/// </summary>
/// <returns></returns>
glm::vec3 Light::GetColor() const
{
	return color;
}
/// <summary>
/// Sets the color.
/// </summary>
/// <param name="val">The value.</param>
void Light::SetColor( glm::vec3 val )
{
	color = val;
}
/// <summary>
/// Gets the range.
/// </summary>
/// <returns></returns>
float Light::GetRange() const
{
	return range;
}
/// <summary>
/// Sets the range.
/// </summary>
/// <param name="val">The value.</param>
void Light::SetRange( float val )
{
	range = val;
}