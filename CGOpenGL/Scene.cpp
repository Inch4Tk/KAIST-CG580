#include "Scene.h"

#include "AppManager.h"
#include "BufferStructs.h"
#include "Camera.h"
#include "Debug.h"
#include "Geometry.h"
#include "GUI.h"
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
		for( std::pair<std::string, Geometry*> g : geometries )
			SDELETE( g.second );
		sceneObjects.clear();
		meshes.clear();
		materials.clear();
		shaders.clear();
		geometries.clear();
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
	sceneObjects.push_back( activeCamera );

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

	RegenerateScene();
	
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

/// <summary>
/// Hackish way to get GUI to work correctly with scene to regenerate cubes and lights when vars change
/// </summary>
void Scene::RegenerateScene()
{
	const ShaderProgram* testShader = GetShader( "Test" );
	const ShaderProgram* phongShader = GetShader( "Phong" );
	const ShaderProgram* clusterPre = GetShader( "ClusterPre" );
	const ShaderProgram* clusterPhong = GetShader( "ClusterPhong" );
	Geometry* g = GetGeometry( "cube.obj" );
	
	GUI* gui = AppManager::GetGUI();

	// Determine regeneration variables
	glm::ivec3 lightLowBounds = -(gui->lights / 2);
	glm::ivec3 lightHighBounds = (gui->lights + 1) / 2;
	glm::ivec3 cubeLowBounds = -(gui->cubes / 2);
	glm::ivec3 cubeHighBounds = (gui->cubes + 1) / 2;

	// Check if cube or light grid is different so we need to complete regen
	if( gui->cubes != oldCubeGrid || gui->lights != oldLightGrid )
	{
		// Most inefficient way to regenerate the scene...
		// Delete everything
		sceneObjects.remove( activeCamera );
		for( SceneObject* so : sceneObjects )
		{
			SDELETE( so );
		}
		sceneObjects.clear();
		sceneObjects.push_back( activeCamera );

		// Create lights
		Light* light = nullptr;
		for( int z = lightLowBounds.z; z < lightHighBounds.z; ++z )
		{
			for( int y = lightLowBounds.y; y < lightHighBounds.y; ++y )
			{
				for( int x = lightLowBounds.x; x < lightHighBounds.x; ++x )
				{
					glm::vec3 randOffsets = glm::vec3( gui->lightOffsets.x * randFloat( -1.0f ),
												   gui->lightOffsets.y * randFloat( -1.0f ),
												   gui->lightOffsets.z * randFloat( -1.0f ) );
					glm::vec3 pos = glm::vec3( x, y, z);
					glm::vec3 color = gui->lightColor;
					if( gui->lightRandomColors )
						color = glm::vec3( randFloat(), randFloat(), randFloat() );
					float rangeVar =  randFloat() * gui->lightRangeVariance;
					light = new Light( randOffsets + pos * gui->lightSpacing,
									   color, 
									   std::max(0.1f, gui->lightRange + rangeVar) );
					sceneObjects.push_back( light );
				}
			}
		}

		// Create scene objects
		SceneObject* obj;
		for( int z = cubeLowBounds.z; z < cubeHighBounds.z; ++z )
		{
			for( int y = cubeLowBounds.y; y < cubeHighBounds.y; ++y )
			{
				for( int x = cubeLowBounds.x; x < cubeHighBounds.x; ++x )
				{
					glm::vec3 randOffsets = glm::vec3( gui->cubeOffsets.x * randFloat( -1.0f ),
													   gui->cubeOffsets.y * randFloat( -1.0f ),
													   gui->cubeOffsets.z * randFloat( -1.0f ) );
					glm::vec3 pos = glm::vec3( x, y, z );
					glm::vec3 scale = glm::vec3( gui->cubeScale.x * randFloat( -1.0f ),
												 gui->cubeScale.y * randFloat( -1.0f ),
												 gui->cubeScale.z * randFloat( -1.0f ) );
					obj = new SceneObject( g );
					obj->position = pos * gui->cubeSpacing + randOffsets;
					obj->scale += scale;
					if( gui->cubeRandRot )
					{
						obj->rotation = glm::quat( glm::vec3(randFloat() * 2.0f * glm::pi<float>(),
												   randFloat() * 2.0f * glm::pi<float>(),
												   randFloat() * 2.0f * glm::pi<float>() ) );
					}
					obj->AddShader( "test", testShader );
					obj->AddShader( "phong", phongShader );
					obj->AddShader( "clusterPre", clusterPre );
					obj->AddShader( "clusterPhong", clusterPhong );
					AppManager::GetObjectManager()->SubscribeRender( obj );
					sceneObjects.push_back( obj );
				}
			}
		}

		// Update old cube and light grid vars
		oldCubeGrid = gui->cubes;
		oldLightGrid = gui->lights;
	}
	else // Only other vars are different just run over the grid and update the rest
	{
		// We abuse the fact that lights are always before scene objects in the current version 
		// of the program, if this gets changed everything will get recked... well npnp
		// TODO: update this to check for type and forward iterators correctly
		auto it = sceneObjects.begin();
		++it; // Skip camera
		// Update lights
		for( int z = lightLowBounds.z; z < lightHighBounds.z; ++z )
		{
			for( int y = lightLowBounds.y; y < lightHighBounds.y; ++y )
			{
				for( int x = lightLowBounds.x; x < lightHighBounds.x; ++x )
				{
					glm::vec3 randOffsets = glm::vec3( gui->lightOffsets.x * randFloat( -1.0f ),
													   gui->lightOffsets.y * randFloat( -1.0f ),
													   gui->lightOffsets.z * randFloat( -1.0f ) );
					glm::vec3 pos = glm::vec3( x, y, z );
					glm::vec3 color = gui->lightColor;
					if( gui->lightRandomColors )
						color = glm::vec3( randFloat(), randFloat(), randFloat() );
					float rangeVar = randFloat() * gui->lightRangeVariance;
					Light* light = dynamic_cast<Light*>(*it);
					light->position = pos * gui->lightSpacing + randOffsets;
					light->SetColor( color );
					light->SetRange( std::max( 0.1f, gui->lightRange + rangeVar ) );

					++it;
				}
			}
		}

		// Create scene objects
		for( int z = cubeLowBounds.z; z < cubeHighBounds.z; ++z )
		{
			for( int y = cubeLowBounds.y; y < cubeHighBounds.y; ++y )
			{
				for( int x = cubeLowBounds.x; x < cubeHighBounds.x; ++x )
				{
					glm::vec3 randOffsets = glm::vec3( gui->cubeOffsets.x * randFloat( -1.0f ),
													   gui->cubeOffsets.y * randFloat( -1.0f ),
													   gui->cubeOffsets.z * randFloat( -1.0f ) );
					glm::vec3 pos = glm::vec3( x, y, z );
					glm::vec3 scale = glm::vec3( gui->cubeScale.x * randFloat( -1.0f ),
												 gui->cubeScale.y * randFloat( -1.0f ),
												 gui->cubeScale.z * randFloat( -1.0f ) );
					SceneObject* obj = *it;
					obj->position = pos * gui->cubeSpacing + randOffsets;
					obj->scale += scale;
					if( gui->cubeRandRot )
					{
						obj->rotation = glm::quat( glm::vec3( randFloat() * 2.0f * glm::pi<float>(),
							randFloat() * 2.0f * glm::pi<float>(),
							randFloat() * 2.0f * glm::pi<float>() ) );
					}

					++it;
				}
			}
		}
	}
}
