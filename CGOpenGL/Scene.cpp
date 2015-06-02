#include "Scene.h"

#include "AppManager.h"
#include "Camera.h"
#include "Geometry.h"
#include "Material.h"
#include "Mesh.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "ShaderProgram.h"

// Include tiny object loader module
#include "tiny_obj_loader.h"

Scene::Scene()
{
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
	activeCamera->MakeFirstPerson( 10.0f, 10.0f, false );
	int winWidth, winHeight;
	glfwGetWindowSize( AppManager::GetWindow(), &winWidth, &winHeight );
	activeCamera->MakePerspective( 60.0f, static_cast<float>(winWidth)/static_cast<float>(winHeight), 0.1f, 1000.0f );

	// Load a shader
	ShaderProgram::InitConfig shaderConfig;
	shaderConfig.vsPath = "Shader/testVshader.glsl";
	shaderConfig.fsPath = "Shader/testFshader.glsl";
	const ShaderProgram* testShader = ShaderProgram::LoadProgram( "TestShader", shaderConfig );

	// Load a cube
	Geometry* g = new Geometry( "cube.obj" );

	// Create a random triangle mesh
	/*glm::vec3 tris[3];
	tris[0] = { 0.0f, 1.0f, 0.0f };
	tris[1] = { 0.0f, 0.0f, 0.0f };
	tris[2] = { 1.0f, 0.0f, 0.0f };

	Mesh* triangleMesh = new Mesh();
	triangleMesh->Initialize( VertexFormatManager::Get3F(), tris, 36, GL_TRIANGLES);
	meshes["TriangleMesh"] = triangleMesh;*/
	
	// Create a scene object containing the mesh and the test shader
	SceneObject* tri = new SceneObject( g, testShader );
	AppManager::GetObjectManager()->SubscribeRender( tri );
	sceneObjects.push_back( tri );
}

/// <summary>
/// Registers the mesh for auto deletion and for easy global access and memory de-duplication. If name already exists nothing happens.
/// </summary>
/// <param name="name">The name.</param>
/// <param name="mesh">The mesh.</param>
void Scene::RegisterMesh( const std::string& name, Mesh* mesh )
{
	if( meshes.count(name) == 0 )
		meshes[name] = mesh;
}

/// <summary>
/// Registers the material for auto deletion and for easy global access and memory de-duplication. If name already exists nothing happens.
/// </summary>
/// <param name="name">The name.</param>
/// <param name="material">The material.</param>
void Scene::RegisterMaterial( const std::string& name, Material* material )
{
	if( materials.count( name ) == 0 )
		materials[name] = material;
}

/// <summary>
/// Registers the shader for auto deletion and for easy global access and memory de-duplication. If name already exists nothing happens.
/// </summary>
/// <param name="name">The name.</param>
/// <param name="shader">The shader.</param>
void Scene::RegisterShader( const std::string& name, ShaderProgram* shader )
{
	if( shaders.count( name ) == 0 )
		shaders[name] = shader;
}

/// <summary>
/// Gets the mesh.
/// </summary>
/// <param name="name">The name.</param>
/// <returns></returns>
Mesh* Scene::GetMesh( const std::string& name )
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
Material* Scene::GetMaterial( const std::string& name )
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
ShaderProgram* Scene::GetShader( const std::string& name )
{
	auto it = shaders.find( name );
	if( it != shaders.end() )
	{
		return it->second;
	}
	return nullptr;
}

/// <summary>
/// Gets the active camera.
/// </summary>
/// <returns></returns>
const Camera* Scene::GetActiveCamera()
{
	return activeCamera;
}
