#pragma once

#include "StandardIncludes.h"

// Forwards
class Camera;
class Mesh;
class SceneObject;
class ShaderProgram;

class Scene
{
public:
	static int Load( const std::string& name );
	static void Unload();

private:
	Scene();
	~Scene();

	static bool loaded;
	static Camera* activeCamera;
	static std::list<SceneObject*> sceneObjects;
	static std::unordered_map<std::string, Mesh*> meshes;
	static std::unordered_map<std::string, ShaderProgram*> shaders;

	static void LoadTestScene();
};

