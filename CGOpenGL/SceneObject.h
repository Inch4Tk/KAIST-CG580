#pragma once

#include "StandardIncludes.h"

// Forwards
class Geometry;
class ShaderProgram;

class SceneObject
{
public:
	SceneObject();
	SceneObject( Geometry* geometry );
	~SceneObject();

	void AddShader( const std::string& shaderName, const ShaderProgram* shader );
	virtual void Update();
	virtual void Render( const std::string& shaderName );

	// Transformation variables
	glm::vec3 position = { 0, 0, 0 }; // Implies world 
	glm::quat rotation = { 1, 0, 0, 0 }; // Identity, w = first component
	glm::vec3 scale = { 1, 1, 1 };

protected:
	Geometry* geometry = nullptr;
	std::unordered_map<std::string, const ShaderProgram*> shaders;
};

