#pragma once

#include "StandardIncludes.h"

// Forwards
class SceneObject;

class ObjectManager
{
public:
	static void ExecUpdate();
	static void ExecRender();

	static void SubscribeUpdate( SceneObject* sub );
	static void SubscribeRender( SceneObject* sub );

private:
	ObjectManager();
	~ObjectManager();

	static std::list<SceneObject*> subsUpdate;
	static std::list<SceneObject*> subsRender;
};

