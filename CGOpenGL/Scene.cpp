#include "Scene.h"

#include "Camera.h"
#include "Mesh.h"
#include "ObjectManager.h"
#include "SceneObject.h"
#include "ShaderProgram.h"

bool Scene::loaded = false;
Camera* Scene::activeCamera = nullptr;
std::list<SceneObject*> Scene::sceneObjects = std::list<SceneObject*>();
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
		for( std::pair<std::string, ShaderProgram*> sp : shaders )
			SDELETE( sp.second );
		sceneObjects.clear();
		meshes.clear();
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

	// Create a random triangle mesh
	glm::vec3 tris[3];
	tris[0] = { 0.0f, 1.0f, 0.0f };
	tris[1] = { 0.0f, 0.0f, 0.0f };
	tris[2] = { 1.0f, 0.0f, 0.0f };

	Mesh* triangleMesh = new Mesh();
	triangleMesh->Initialize( VertexFormatManager::Get3F(), tris, 36, GL_TRIANGLES);
	meshes["TriangleMesh"] = triangleMesh;
	
	// Create a scene object containing the mesh and the test shader
	SceneObject* tri = new SceneObject( triangleMesh, testShader );
	ObjectManager::SubscribeRender( tri );
	sceneObjects.push_back( tri );
}
