#include "ObjectManager.h"

#include "SceneObject.h"

std::list<SceneObject*> ObjectManager::subsUpdate = std::list<SceneObject*>();
std::list<SceneObject*> ObjectManager::subsRender = std::list<SceneObject*>();

ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}

/// <summary>
/// Executes the update calls on all subscribed objects.
/// </summary>
void ObjectManager::ExecUpdate()
{
	for( SceneObject* ro : subsUpdate )
		ro->Update();
}

/// <summary>
/// Executes the render calls on all subscribed objects.
/// </summary>
void ObjectManager::ExecRender()
{
	for( SceneObject* ro : subsRender )
		ro->Render();
}

/// <summary>
/// Subscribes sub to update calls.
/// </summary>
/// <param name="sub">The sub.</param>
void ObjectManager::SubscribeUpdate( SceneObject* sub )
{
	subsUpdate.push_back( sub );
}

/// <summary>
/// Subscribes sub to render calls.
/// </summary>
/// <param name="sub">The sub.</param>
void ObjectManager::SubscribeRender( SceneObject* sub )
{
	subsUpdate.push_back( sub );
}
