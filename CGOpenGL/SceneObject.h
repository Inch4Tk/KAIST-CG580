#pragma once

// Forwards
class Mesh;
class ShaderProgram;

class SceneObject
{
public:
	SceneObject();
	~SceneObject();

	virtual void Update();
	virtual void Render();

protected:
	Mesh* geometry = nullptr;
	ShaderProgram* shader = nullptr;
};

