#pragma once

class SceneObject
{
public:
	SceneObject();
	~SceneObject();

	virtual void Update();
	virtual void Render();
};

