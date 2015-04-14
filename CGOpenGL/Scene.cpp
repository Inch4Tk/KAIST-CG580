#include "Scene.h"

#include "Camera.h"
#include "Geometry.h"
#include "Material.h"
#include "Mesh.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "ShaderProgram.h"

// Include tiny object loader module
#include "tiny_obj_loader.h"


bool Scene::loaded = false;
Camera* Scene::activeCamera = nullptr;
std::list<SceneObject*> Scene::sceneObjects = std::list<SceneObject*>();
std::unordered_map<std::string, Material*> Scene::materials = std::unordered_map<std::string, Material*>();
std::unordered_map<std::string, Mesh*> Scene::meshes = std::unordered_map<std::string, Mesh*>();
std::unordered_map<std::string, ShaderProgram*> Scene::shaders = std::unordered_map<std::string, ShaderProgram*>();

Scene::Scene()
{
}

Scene::~Scene()
{
}

/// <summary>
/// Loads the scene with the specified name.
/// </summary>
/// <param name="name">The name.</param>
/// <returns></returns>
int Scene::Load( const std::string& name )
{
	// Potentially unload a loaded scene beforehand
	Unload();

	// Load the correct scene
	LoadTestScene(); // this has to be set as the else as soon as we try loading a different scene

	loaded = true;
	return 0;
}

/// <summary>
/// Unloads this instance.
/// </summary>
void Scene::Unload()
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
/// Loads a test scene.
/// </summary>
void Scene::LoadTestScene()
{
	// Load a shader
	ShaderProgram* testShader = new ShaderProgram();
	ShaderProgram::InitConfig shaderConfig;
	shaderConfig.vsPath = "Shader/testVshader.glsl";
	shaderConfig.fsPath = "Shader/testFshader.glsl";
	testShader->LoadProgram(shaderConfig);
	shaders["TestShader"] = testShader;

	// Load a cube
	Geometry* g = new Geometry( "cube.obj" );
	// TODO FIX GEOMETRY LOADING MESHES MULTITIMES

	// Create a random triangle mesh
	glm::vec3 tris[3];
	tris[0] = { 0.0f, 1.0f, 0.0f };
	tris[1] = { 0.0f, 0.0f, 0.0f };
	tris[2] = { 1.0f, 0.0f, 0.0f };

	Mesh* triangleMesh = new Mesh();
	triangleMesh->Initialize( VertexFormatManager::Get3F(), tris, 36, GL_TRIANGLES);
	meshes["TriangleMesh"] = triangleMesh;
	
	// Create a scene object containing the mesh and the test shader
	SceneObject* tri = new SceneObject( g, testShader );
	ObjectManager::SubscribeRender( tri );
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
