#include "Camera.h"


Camera::Camera()
{
}


Camera::~Camera()
{
}


void Camera::Update()
{
	// Handle movements

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
/// <param name="fovy">The fovy.</param>
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
/// <param name="movementSpeed">The movement speed.</param>
/// <param name="turnSpeed">The turn speed.</param>
/// <param name="lockedToMouse">if set to <c>true</c> [locked to mouse].</param>
void Camera::MakeFirstPerson( float movementSpeed, float turnSpeed, bool lockedToMouse )
{
	this->movementSpeed = movementSpeed;
	this->turnSpeed = turnSpeed;
	this->lockedToMouse = lockedToMouse;
	movementType = MovementType::FirstPerson;
}
