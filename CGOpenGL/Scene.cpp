#include "Scene.h"

#include "Camera.h"
#include "SceneObject.h"

bool Scene::loaded = false;
Camera* Scene::activeCamera = nullptr;
std::list<SceneObject*> Scene::sceneObjects = std::list<SceneObject*>();

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