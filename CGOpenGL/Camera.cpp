#include "Camera.h"

#include "AppManager.h"
#include "Debug.h"
#include "Input.h"
#include "Time.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}


void Camera::Update()
{
	// Handle movements
	if( movementType == MovementType::FirstPerson )
	{
		UpdateFirstPerson();
	}

	// Calculate view/projection matrices
	view = glm::lookAt( position, position + viewDir, globalUpDir );
	if( projectionType == ProjectionType::Perspective )
	{
		projection = glm::perspective( fovy, aspect, nearPlane, farPlane );
	}
	else if( projectionType == ProjectionType::Orthographic )
	{
		projection = glm::ortho( left, right, bottom, top, nearPlane, farPlane );
	}
	viewProjection = projection * view; // First apply view then projection
}

void Camera::Render()
{
	// Empty, because we don't want to render the camera, but still need to override the default render behavior
}

/// <summary>
/// Gets the view.
/// </summary>
/// <returns></returns>
glm::mat4 Camera::GetView() const
{
	return view;
}

/// <summary>
/// Gets the projection.
/// </summary>
/// <returns></returns>
glm::mat4 Camera::GetProjection() const
{
	return projection;
}

/// <summary>
/// Gets the view projection.
/// </summary>
/// <returns></returns>
glm::mat4 Camera::GetViewProjection() const
{
	return viewProjection;
}

/// <summary>
/// Makes the camera perspective.
/// </summary>
/// <param name="fovy">The fovy in radians.</param>
/// <param name="aspect">The aspect.</param>
/// <param name="nearPlane">The near plane.</param>
/// <param name="farPlane">The far plane.</param>
void Camera::MakePerspective( float fovy, float aspect, float nearPlane, float farPlane )
{
	this->fovy = fovy;
	this->aspect = aspect;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	projectionType = ProjectionType::Perspective;
}

/// <summary>
/// Makes the camera orthographic.
/// </summary>
/// <param name="left">The left.</param>
/// <param name="right">The right.</param>
/// <param name="bottom">The bottom.</param>
/// <param name="top">The top.</param>
/// <param name="nearPlane">The near plane.</param>
/// <param name="farPlane">The far plane.</param>
void Camera::MakeOrthographic( float left, float right, float bottom, float top, float nearPlane, float farPlane )
{
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;
	projectionType = ProjectionType::Orthographic;
}

/// <summary>
/// Makes the camera first person.
/// </summary>
/// <param name="movementSpeed">The movement speed in m/s.</param>
/// <param name="turnSpeed">The turn speed in screens (from 0 to 1.0) expressed in rad.</param>
/// <param name="yLimitAngle">The y limit angle in radians. Limits the viewing range to prevent flipping.</param>
/// <param name="lockedToMouse">if set to <c>true</c> [locked to mouse].</param>
void Camera::MakeFirstPerson( float movementSpeed, float turnSpeed, float yLimitAngle, bool lockedToMouse )
{
	this->movementSpeed = movementSpeed;
	this->turnSpeed = turnSpeed;
	this->lockedToMouse = lockedToMouse;
	downPitchLimit = yLimitAngle;
	upPitchLimit = glm::pi<float>() - yLimitAngle;
	movementType = MovementType::FirstPerson;
}

/// <summary>
/// Updates the camera like in a first person view.
/// </summary>
void Camera::UpdateFirstPerson()
{
	Input* input = AppManager::GetInput();

	// Delta speed
	float deltaMS = movementSpeed * static_cast<float>(AppManager::GetTime()->GetDelta());
	// Update the position
	position += rightDir * (input->GetInputAxisX() * deltaMS);
	position += viewDir * (input->GetInputAxisY() * deltaMS);

	// Rotate the view around the right axis
	float pitchingAngle = -input->GetMouseDeltaY() * turnSpeed;
	// Limit pitching to prevent flipping
	float curYAngle = glm::acos( viewDir.y );
	float requestedPitch = curYAngle - pitchingAngle;

	if( requestedPitch > upPitchLimit )
		pitchingAngle = upPitchLimit - curYAngle;
	else if( requestedPitch < downPitchLimit )
		pitchingAngle = downPitchLimit - curYAngle;

	viewDir = glm::rotate( viewDir, pitchingAngle, rightDir );
	// Rotate the view around the global up axis
	viewDir = glm::rotate( viewDir, -input->GetMouseDeltaX() * turnSpeed, globalUpDir );

	// Re-normalize the right dir vector
	rightDir = glm::normalize( glm::cross( viewDir, globalUpDir ) );
}
