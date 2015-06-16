#include "Scene.h"

#include "AppManager.h"
#include "BufferStructs.h"
#include "Camera.h"
#include "Debug.h"
#include "Geometry.h"
#include "Light.h"
#include "Material.h"
#include "Mesh.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "ShaderProgram.h"

// Include tiny object loader module
#include "tiny_obj_loader.h"

Scene::Scene()
{
	// Create the default material
	RegisterMaterial( "Default", new Material( glm::vec3( 1, 1, 1 ), glm::vec3( 1, 1, 1 ), glm::vec3( 1, 1, 1 ), 15.0f ) );
}

Scene::~Scene()
{
	if( loaded )
	{
		for( SceneObject* so : sceneObjects )
			SDELETE( so );
		for( std::pair<std::string, Mesh*> m : meshes )
			SDELETE( m.second );
		for( std::pair<std::string, Material*> mat : materials )
			SDELETE( mat.second );
		for( std::pair<std::string, ShaderProgram*> sp : shaders )
			SDELETE( sp.second );
		sceneObjects.clear();
		meshes.clear();
		materials.clear();
		shaders.clear();
		loaded = false;
	}
}


/// <summary>
/// Loads the scene.
/// </summary>
/// <param name="name">The name.</param>
void Scene::LoadScene( const std::string& name )
{
	// Only load unloaded scenes
	if( loaded )
		return;

	// Load the correct scene
	LoadTestScene(); // this has to be set as the else as soon as we try loading a different scene
	loaded = true;
}


/// <summary>
/// Loads a test scene.
/// </summary>
void Scene::LoadTestScene()
{
	// Create a default projective camera
	activeCamera = new Camera();
	activeCamera->position = glm::vec3( 0, 0, -5 );
	activeCamera->MakeFirstPerson( 5.0f, glm::radians(360.0f), glm::radians(10.0f), true );
	activeCamera->MakePerspective( Config::FOV_Y, Config::ASPECT, Config::NEAR_PLANE, Config::FAR_PLANE );

	// Load a shader
	ShaderProgram::InitConfig shaderConfig;
	shaderConfig.vsPath = "Shader/testVshader.glsl";
	shaderConfig.fsPath = "Shader/testFshader.glsl";
	const ShaderProgram* testShader = ShaderProgram::LoadProgram( "Test", shaderConfig, 
																  BindSlots::defaultBindSlots );
	shaderConfig.vsPath = "Shader/PhongVshader.glsl";
	shaderConfig.fsPath = "Shader/PhongFshader.glsl";
	const ShaderProgram* phongShader = ShaderProgram::LoadProgram( "Phong", shaderConfig, 
																   BindSlots::defaultBindSlots );
	shaderConfig.vsPath = "Shader/ClusterPrepassV.glsl";
	shaderConfig.fsPath = "Shader/ClusterPrepassF.glsl";
	const ShaderProgram* clusterPre = ShaderProgram::LoadProgram( "ClusterPre", shaderConfig,
																   BindSlots::defaultBindSlots );
	shaderConfig.vsPath = "Shader/ClusterForwardV.glsl";
	shaderConfig.fsPath = "Shader/ClusterForwardF.glsl";
	const ShaderProgram* clusterPhong = ShaderProgram::LoadProgram( "ClusterPhong", shaderConfig,
																   BindSlots::defaultBindSlots );
	
	// Add some lights
	Light* light = nullptr;
	/*light = new Light( glm::vec3( 3, 0, 0 ), glm::vec3( randFloat(), randFloat(), randFloat() ), 5.0f );
	sceneObjects.push_back( light );*/

	for( int z = -2; z < 3; ++z )
	{
		for( int y = -1; y < 2; ++y )
		{
			for( int x = -2; x < 3; ++x )
			{
				light = new Light( glm::vec3( x * 2.5, y, z * 2.5 ), glm::vec3( randFloat(), randFloat(), randFloat() ), 5.0f );
				sceneObjects.push_back( light );
			}
		}
	}

	// Load a cube
	std::string geomName = "cube.obj";
	Geometry* g = GetGeometry( geomName );
	if( g == nullptr )
	{
		g = new Geometry( geomName );
		RegisterGeometry( geomName, g );
	}

	// Create a random triangle mesh
	/*glm::vec3 tris[3];
	tris[0] = { 0.0f, 1.0f, 0.0f };
	tris[1] = { 0.0f, 0.0f, 0.0f };
	tris[2] = { 1.0f, 0.0f, 0.0f };

	Mesh* triangleMesh = new Mesh();
	triangleMesh->Initialize( VertexFormatManager::Get3F(), tris, 36, GL_TRIANGLES);
	meshes["TriangleMesh"] = triangleMesh;*/
	
	// Create a scene object containing the mesh and the test shader
	SceneObject* obj = new SceneObject( g );
	obj->AddShader( "test", testShader );
	obj->AddShader( "phong", phongShader );
	obj->AddShader( "clusterPre", clusterPre );
	obj->AddShader( "clusterPhong", clusterPhong );
	AppManager::GetObjectManager()->SubscribeRender( obj );
	sceneObjects.push_back( obj );
}

/// <summary>
/// Registers the mesh for auto deletion and for easy global access and memory de-duplication. Returns the same mesh.
/// If name already exists the passed in mesh will be deleted and the existing will be returned.
/// </summary>
/// <param name="name">The name.</param>
/// <param name="mesh">The mesh.</param>
Mesh* Scene::RegisterMesh( const std::string& name, Mesh* mesh )
{
	auto it = meshes.find( name );
	if( it == meshes.end() )
	{
		meshes[name] = mesh;
		return mesh;
	}
	SDELETE( mesh );
	return it->second;
}

/// <summary>
/// Registers the material for auto deletion and for easy global access and memory de-duplication. Returns the same material.
/// If name already exists the passed in mesh will be deleted and the existing will be returned.
/// </summary>
/// <param name="name">The name.</param>
/// <param name="material">The material.</param>
Material* Scene::RegisterMaterial( const std::string& name, Material* material )
{
	auto it = materials.find( name );
	if( it == materials.end() )
	{
		materials[name] = material;
		return material;
	}
	SDELETE( material );
	return it->second;
}

/// <summary>
/// Registers the shader for auto deletion and for easy global access and memory de-duplication. Returns the same shader.
/// If name already exists the passed in mesh will be deleted and existing will be returned.
/// </summary>
/// <param name="name">The name.</param>
/// <param name="shader">The shader.</param>
ShaderProgram* Scene::RegisterShader( const std::string& name, ShaderProgram* shader )
{
	auto it = shaders.find( name );
	if( it == shaders.end() )
	{
		shaders[name] = shader;
		return shader;
	}
	SDELETE( shader );
	return it->second;
}

/// <summary>
/// Registers the geometry for auto deletion and for easy global access and memory de-duplication. Returns the same geometry.
/// If name already exists the passed in geometry will be deleted and existing will be returned.
/// </summary>
/// <param name="name">The name.</param>
/// <param name="geom">The geom.</param>
/// <returns></returns>
Geometry* Scene::RegisterGeometry( const std::string& name, Geometry* geom )
{
	auto it = geometries.find( name );
	if( it == geometries.end() )
	{
		geometries[name] = geom;
		return geom;
	}
	SDELETE( geom );
	return it->second;
}

/// <summary>
/// Gets the mesh.
/// </summary>
/// <param name="name">The name.</param>
/// <returns></returns>
Mesh* Scene::GetMesh( const std::string& name ) const
{
	auto it = meshes.find( name );
	if( it != meshes.end() )
	{
		return it->second;
	}
	return nullptr;
}

/// <summary>
/// Gets the material.
/// </summary>
/// <param name="name">The name.</param>
/// <returns></returns>
Material* Scene::GetMaterial( const std::string& name ) const
{
	auto it = materials.find( name );
	if( it != materials.end() )
	{
		return it->second;
	}
	return nullptr;
}

/// <summary>
/// Gets the shader.
/// </summary>
/// <param name="name">The name.</param>
/// <returns></returns>
ShaderProgram* Scene::GetShader( const std::string& name ) const
{
	auto it = shaders.find( name );
	if( it != shaders.end() )
	{
		return it->second;
	}
	return nullptr;
}

/// <summary>
/// Gets the geometry.
/// </summary>
/// <param name="name">The name.</param>
/// <returns></returns>
Geometry* Scene::GetGeometry( const std::string& name ) const
{
	auto it = geometries.find( name );
	if( it != geometries.end() )
	{
		return it->second;
	}
	return nullptr;
}

/// <summary>
/// Gets the active camera.
/// </summary>
/// <returns></returns>
Camera* Scene::GetActiveCamera() const
{
	return activeCamera;
}
