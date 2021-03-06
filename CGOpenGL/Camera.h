#pragma once

#include "SceneObject.h"

class Camera :
	public SceneObject
{
public:
	Camera();
	~Camera();

	// Scene object overrides
	void Update();
	void Render( const std::string& shaderName );

	// Change camera projection types
	void MakePerspective( float fovy, float aspect, float nearPlane, float farPlane );
	void MakeOrthographic( float left, float right, float bottom, float top, float nearPlane, float farPlane);

	// Change camera movement types/behavior
	void MakeFirstPerson( float movementSpeed, float turnSpeed, float yLimitAngle, bool lockedToMouse );
	void AddMovespeed( float moveSpeed );
	void MultMovespeed( float multiplier );
	void ToggleMouseLock();

	// Getters
	float GetFOV() const;
	float GetFarPlane() const;
	float GetNearPlane() const;
	float GetMovespeed() const;
	glm::mat4 GetView() const;
	glm::mat4 GetProjection() const;
	glm::mat4 GetViewProjection() const;

protected:
	// -------------------
	// Settings
	// -------------------
	// Movement related
	enum MovementType
	{
		NoMovement,
		FirstPerson
	};
	MovementType movementType = MovementType::NoMovement; // 0 = Nothing, 1 = FirstPerson

	float movementSpeed = 0;
	float turnSpeed = 0;
	float upPitchLimit = 0;
	float downPitchLimit = 0;
	bool lockedToMouse = false;

	void UpdateFirstPerson();

	// Projection related
	enum ProjectionType
	{
		NoProjection,
		Perspective,
		Orthographic
	};
	ProjectionType projectionType = ProjectionType::NoProjection; // 0 = Nothing, 1 = Perspective, 2 = Orthographic

	float left;
	float right;
	float bottom;
	float top;
	float fovy;
	float aspect;
	float nearPlane;
	float farPlane;
	
	// Transformation related
	glm::vec3 globalUpDir = { 0, 1, 0 };
	glm::vec3 viewDir = { 0, 0, 1 };
	glm::vec3 rightDir = { -1, 0, 0 };

	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 viewProjection;

};

