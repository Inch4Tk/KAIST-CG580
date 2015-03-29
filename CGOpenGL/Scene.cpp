#include "Scene.h"

#include "Camera.h"
#include "Mesh.h"
#include "SceneObject.h"
#include "ShaderProgram.h"

bool Scene::loaded = false;
Camera* Scene::activeCamera = nullptr;
std::list<SceneObject*> Scene::sceneObjects = std::list<SceneObject*>();
std::unordered_map<std::string, Mesh*> Scene::meshes = std::unordered_map<std::string, Mesh*>();
std::unordered_map<std::string, ShaderProgram*> Scene::shaders = std::unordered_map<std::string, ShaderProgram*>();

Scene::Scene()
{
}

Scene::~Scene()
{
}

/// <summary>
/// Loads the scene with the specified name.
/// </summary>
/// <param name="name">The name.</param>
/// <returns></returns>
int Scene::Load( const std::string& name )
{
	// Potentially unload a loaded scene beforehand
	Unload();

	// Load the correct scene
	LoadTestScene(); // this has to be set as the else as soon as we try loading a different scene

	loaded = true;
	return 0;
}

/// <summary>
/// Unloads this instance.
/// </summary>
void Scene::Unload()
{
	if( loaded )
	{
		for( SceneObject* so : sceneObjects )
			SDELETE( so );
		loaded = false;
	}
}

/// <summary>
/// Loads a test scene.
/// </summary>
void Scene::LoadTestScene()
{

}
