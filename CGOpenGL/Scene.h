#pragma once

#include "StandardIncludes.h"

// Forwards
class Camera;
class Material;
class Mesh;
class SceneObject;
class ShaderProgram;

class Scene
{
	friend class AppManager;
public:
	void RegisterMesh( const std::string& name, Mesh* mesh );
	void RegisterMaterial( const std::string& name, Material* material );
	void RegisterShader( const std::string& name, ShaderProgram* shader );

	const Camera* GetActiveCamera();
	Mesh* GetMesh( const std::string& name );
	Material* GetMaterial( const std::string& name );
	ShaderProgram* GetShader( const std::string& name );

private:
	Scene();
	~Scene();
	
	void LoadScene( const std::string& name );

	bool loaded = false;
	Camera* activeCamera;
	std::list<SceneObject*> sceneObjects;
	std::unordered_map<std::string, Mesh*> meshes;
	std::unordered_map<std::string, Material*> materials;
	std::unordered_map<std::string, ShaderProgram*> shaders;

	void LoadTestScene();
};

