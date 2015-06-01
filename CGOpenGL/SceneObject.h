#pragma once

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

protected:
	Geometry* geometry = nullptr;
	const ShaderProgram* shader = nullptr;
};

