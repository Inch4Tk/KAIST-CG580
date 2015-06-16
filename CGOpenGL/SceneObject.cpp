#include "SceneObject.h"

#include "AppManager.h"
#include "Debug.h"
#include "Geometry.h"
#include "ObjectManager.h"
#include "ShaderProgram.h"

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(Geometry* geometry) : geometry(geometry)
{
}


SceneObject::~SceneObject()
{
	AppManager::GetObjectManager()->UnSubscribeUpdate( this );
	AppManager::GetObjectManager()->UnSubscribeRender( this );
}

/// <summary>
/// Adds a shader.
/// </summary>
/// <param name="shaderName">Name of the shader.</param>
/// <param name="shader">The shader.</param>
void SceneObject::AddShader( const std::string& shaderName, const ShaderProgram* shader )
{
	shaders[shaderName] = shader;
}

/// <summary>
/// Updates this instance.
/// </summary>
void SceneObject::Update()
{
	// Empty default update
}

/// <summary>
/// Renders this instance.
/// </summary>
void SceneObject::Render(const std::string& shaderName)
{
	// Bind the shader
	auto shader = shaders.find( shaderName );
	if( shader != shaders.end() )
		shader->second->BindShader();
	else
		return; // Don't render anything if there is no shader

	// Bind the uniforms
	glm::mat4 rot = glm::mat4_cast( rotation );
	glm::mat4 worldMatrix = glm::translate( glm::mat4(1.0f), position ) * 
		glm::mat4_cast( rotation ) * 
		glm::scale( glm::mat4(1.0f), scale );
	glUniformMatrix4fv( shader->second->GetUniformLocation( "worldMatrix" ), 1, false, &worldMatrix[0][0] );

	// Draw
	geometry->Draw( shader->second->GetBindSlots() );
}
