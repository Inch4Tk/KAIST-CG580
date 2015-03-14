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

void ObjectManager::ExecUpdate()
{
	for( SceneObject* ro : subsUpdate )
		ro->Update();
}

void ObjectManager::ExecRender()
{
	for( SceneObject* ro : subsRender )
		ro->Render();
}

void ObjectManager::SubscribeUpdate( SceneObject* sub )
{
	subsUpdate.push_back( sub );
}

void ObjectManager::SubscribeRender( SceneObject* sub )
{
	subsUpdate.push_back( sub );
}
