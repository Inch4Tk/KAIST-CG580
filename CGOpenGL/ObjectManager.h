#pragma once

#include "StandardIncludes.h"
#include "BufferStructs.h"
#include "GLBuffer.h"

// Forwards
class Light;
class SceneObject;

class ObjectManager
{
public:
	friend class AppManager;

	void SubscribeUpdate( SceneObject* sub );
	void SubscribeRender( SceneObject* sub );
	void UnSubscribeUpdate( SceneObject* sub );
	void UnSubscribeRender( SceneObject* sub );
	void AddLight( Light* light );
	void RemoveLight( Light* light );

	void BindPerFrameUniformBuffer( const std::unordered_map<std::string, uint32_t>& uniSlots );

	bool IsClusteringActive() const;
	void ToggleClustering();
private:
	ObjectManager();
	~ObjectManager();

	void ExecUpdate();
	void ExecRender();
	
	std::list<SceneObject*> subsUpdate;
	std::list<SceneObject*> subsRender;
	std::list<Light*> sceneLights;

	GLBuffer<ShaderGlobals_Std140>* uniBufferGlobals;
	GLBuffer<ShaderLight_Std140>* uniBufferLights;

	// Clustered lighting
	bool clusteringActive = true;
	void BuildCluster( float invNear, float invLogSubDiv );
	void CalcClusterCPU( float invNear, float invLogSubDiv );
	GLBuffer<uint32_t>* uniTexBufUsedClusters;
	GLBuffer<glm::uvec2>* uniTexBufClusters;
	GLBuffer<int32_t>* uniTexBufClusterLightIdx;
};

