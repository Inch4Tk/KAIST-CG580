#pragma once

// Forwards
class Mesh;
class ShaderProgram;

class SceneObject
{
public:
	SceneObject();
	SceneObject( Mesh* geometry, ShaderProgram* shader );
	~SceneObject();

	virtual void Update();
	virtual void Render();

protected:
	Mesh* geometry = nullptr;
	ShaderProgram* shader = nullptr;
};

