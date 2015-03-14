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

int Scene::Load(std::string name)
{
	// Potentially unload a loaded scene beforehand
	Unload();

	// Load the correct scene

	loaded = true;
	return 0;
}

void Scene::Unload()
{
	if( loaded )
	{
		for( SceneObject* so : sceneObjects )
			SDELETE( so );
		loaded = false;
	}
}