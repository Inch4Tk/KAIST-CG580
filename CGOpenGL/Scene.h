#pragma once

#include "StandardIncludes.h"

// Forwards
class Camera;
class Geometry;
class Material;
class Mesh;
class SceneObject;
class ShaderProgram;

class Scene
{
	friend class AppManager;
public:
	Geometry* RegisterGeometry( const std::string& name, Geometry* geom );
	Mesh* RegisterMesh( const std::string& name, Mesh* mesh );
	Material* RegisterMaterial( const std::string& name, Material* material );
	ShaderProgram* RegisterShader( const std::string& name, ShaderProgram* shader );

	Camera* GetActiveCamera() const;
	Geometry* GetGeometry( const std::string& name ) const;
	Mesh* GetMesh( const std::string& name ) const;
	Material* GetMaterial( const std::string& name ) const;
	ShaderProgram* GetShader( const std::string& name ) const;

private:
	Scene();
	~Scene();
	
	void LoadScene( const std::string& name );

	bool loaded = false;
	Camera* activeCamera;
	std::list<SceneObject*> sceneObjects;
	std::unordered_map<std::string, Geometry*> geometries;
	std::unordered_map<std::string, Mesh*> meshes;
	std::unordered_map<std::string, Material*> materials;
	std::unordered_map<std::string, ShaderProgram*> shaders;

	void LoadTestScene();
};

