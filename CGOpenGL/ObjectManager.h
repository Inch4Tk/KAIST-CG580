#pragma once

#include "StandardIncludes.h"

// Forwards
class SceneObject;

class ObjectManager
{
public:
	friend class AppManager;

	void SubscribeUpdate( SceneObject* sub );
	void SubscribeRender( SceneObject* sub );

private:
	ObjectManager();
	~ObjectManager();

	void ExecUpdate();
	void ExecRender();

	std::list<SceneObject*> subsUpdate;
	std::list<SceneObject*> subsRender;
};

