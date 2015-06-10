#pragma once

#include "StandardIncludes.h"
#include "BufferStructs.h"
#include "GLBuffer.h"

// Forwards
class SceneObject;

class ObjectManager
{
public:
	friend class AppManager;

	void SubscribeUpdate( SceneObject* sub );
	void SubscribeRender( SceneObject* sub );

	void BindPerFrameUniformBuffer( const std::unordered_map<std::string, uint32_t>& uniSlots );

private:
	ObjectManager();
	~ObjectManager();

	void ExecUpdate();
	void ExecRender();

	std::list<SceneObject*> subsUpdate;
	std::list<SceneObject*> subsRender;

	GLBuffer<ShaderGlobals_Std140>* uniBufferGlobals;
};

