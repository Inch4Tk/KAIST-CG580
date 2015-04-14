#pragma once

// Forwards
class Geometry;
class ShaderProgram;

class SceneObject
{
public:
	SceneObject();
	SceneObject( Geometry* geometry, ShaderProgram* shader );
	~SceneObject();

	virtual void Update();
	virtual void Render();

protected:
	Geometry* geometry = nullptr;
	ShaderProgram* shader = nullptr;
};

