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
public:
	static int Load( const std::string& name );
	static void Unload();

	static void RegisterMesh( const std::string& name, Mesh* mesh );
	static void RegisterMaterial( const std::string& name, Material* material );
	static void RegisterShader( const std::string& name, ShaderProgram* shader );

	static Mesh* GetMesh( const std::string& name );
	static Material* GetMaterial( const std::string& name );
	static ShaderProgram* GetShader( const std::string& name );

private:
	Scene();
	~Scene();

	static bool loaded;
	static Camera* activeCamera;
	static std::list<SceneObject*> sceneObjects;
	static std::unordered_map<std::string, Mesh*> meshes;
	static std::unordered_map<std::string, Material*> materials;
	static std::unordered_map<std::string, ShaderProgram*> shaders;

	static void LoadTestScene();
};

