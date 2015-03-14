#pragma once

#include "StandardIncludes.h"

// Forwards
class Camera;
class SceneObject;

class Scene
{
public:
	static int Load(std::string name);
	static void Unload();

private:
	Scene();
	~Scene();

	static bool loaded;
	static Camera* activeCamera;
	static std::list<SceneObject*> sceneObjects;
};

