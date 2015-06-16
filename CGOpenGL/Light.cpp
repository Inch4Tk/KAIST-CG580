#include "Light.h"

#include "AppManager.h"
#include "Camera.h"
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


/// <summary>
/// Gets the transformed aabb. Only works with affine transformations. If scaling is applied it will not work on range.
/// </summary>
/// <param name="transform">The transform.</param>
/// <returns></returns>
AABB Light::GetTransformedAABB( glm::mat4& transform ) const
{
	glm::vec3 tPos = glm::vec3( transform * glm::vec4( position, 1.0f ) );
	return AABB( tPos - 0.5f * range, glm::vec3( range ) );
}

/// <summary>
/// Gets the aabb.
/// </summary>
/// <returns></returns>
AABB Light::GetAABB() const
{
	return AABB( position - 0.5f * range, glm::vec3( range ) );
}

/// <summary>
/// Gets the cluster extents.
/// </summary>
/// <returns></returns>
std::pair<glm::uvec3, glm::uvec3> Light::GetClusterExtents( Camera* cam, float invNear, float invLogSubDiv )
{
	// Check if extents were already calculated before
	if( extentsCached )
		return cachedClusterExtents;

	// Check if the light is actually in front of the camera
	AABB aabb = GetTransformedAABB( cam->GetView() );
	if( aabb.positionBLB.z > 0 && aabb.maxExtents.z > 0 )
	{
		extentsCached = true;
		cachedClusterExtents = std::pair<glm::uvec3, glm::uvec3>( { 0, 0, 0 }, { 0, 0, 0 } );
		return cachedClusterExtents;
	}
	else if( aabb.maxExtents.z > 0 )
	{
		aabb.maxExtents.z = 0;
	}

	auto winDim = AppManager::GetWindowDimensions();
	// Get max and min screen position
	// Min
	glm::vec4 posMin = cam->GetProjection() * glm::vec4( aabb.positionBLB, 1.0f );
	posMin /= posMin.w; // homogenize
	posMin = (posMin + glm::vec4( 1.0f )) * 0.5f; // Normalize to 0-1
	// Max
	glm::vec4 posMax = cam->GetProjection() * glm::vec4( aabb.maxExtents, 1.0f );
	posMax /= posMax.w; // homogenize
	posMax = (posMax + glm::vec4( 1.0f )) * 0.5f; // Normalize to 0-1

	// Make checks for out of bounds
	uint32_t xAdd = 1;
	uint32_t yAdd = 1;
	if( posMax.x < 0.0f && posMin.x < 0.0f )
		xAdd = 0;
	if( posMax.x > 1.0f && posMin.x > 1.0f )
		xAdd = 0;
	if( posMax.y < 0.0f && posMin.y < 0.0f )
		yAdd = 0;
	if( posMax.x > 1.0f && posMin.y > 1.0f )
		yAdd = 0;

	// Clamping and scaling to window dim
	posMin = glm::clamp( posMin, glm::vec4( 0 ), glm::vec4( 1 ) ); // Clamp to range
	posMin.x *= winDim.first; // Scale to window dim
	posMin.y *= winDim.second;
	posMax = glm::clamp( posMax, glm::vec4( 0 ), glm::vec4( 1 ) ); // Clamp to range
	posMax.x *= winDim.first; // Scale to window dim
	posMax.y *= winDim.second;

	glm::uvec3 clusterIDmin;
	glm::uvec3 clusterIDmax;

	// Calculate the cluster minimum and maximums
	clusterIDmin.x = static_cast<uint32_t>(posMin.x) / Config::DIM_TILES_X;
	clusterIDmin.y = static_cast<uint32_t>(posMin.y) / Config::DIM_TILES_Y;
	clusterIDmin.z = static_cast<uint32_t>(log( -aabb.positionBLB.z * invNear ) * invLogSubDiv);
	clusterIDmax.x = static_cast<uint32_t>(posMax.x) / Config::DIM_TILES_X + xAdd;
	clusterIDmax.y = static_cast<uint32_t>(posMax.y) / Config::DIM_TILES_Y + yAdd;
	clusterIDmax.z = static_cast<uint32_t>(log( -aabb.maxExtents.z * invNear ) * invLogSubDiv) + 1;

	// Assert everything
	ASSERT( clusterIDmin.x <= clusterIDmax.x );
	ASSERT( clusterIDmin.y <= clusterIDmax.y );
	ASSERT( clusterIDmin.z <= clusterIDmax.z );

	extentsCached = true;
	cachedClusterExtents = std::pair<glm::uvec3, glm::uvec3>( clusterIDmin, clusterIDmax );
	return cachedClusterExtents;
}

/// <summary>
/// Updates this instance. Used to clear the extent cluster
/// </summary>
void Light::Update()
{
	extentsCached = false;
}
