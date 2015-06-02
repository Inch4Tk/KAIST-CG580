#pragma once

#include "StandardIncludes.h"

// Forwards
class Geometry;
class ShaderProgram;

class SceneObject
{
public:
	SceneObject();
	SceneObject( Geometry* geometry, const ShaderProgram* shader );
	~SceneObject();

	virtual void Update();
	virtual void Render();

	// Transformation variables
	glm::vec3 position = { 0, 0, 0 }; // Implies world 
	glm::quat rotation = { 0, 0, 0, 1 }; // Identity, w = last component
	glm::vec3 scale = { 1, 1, 1 };

protected:
	Geometry* geometry = nullptr;
	const ShaderProgram* shader = nullptr;

	// Uniform locations mapping the shader variables 
	GLint uniWorldMatrix = -1;
	GLint uniViewMatrix = -1; // These 3 are highly inefficient and should be grouped into a uniform buffer
	GLint uniProjectionMatrix = -1;
	GLint uniViewProjectionMatrix = -1;
};

