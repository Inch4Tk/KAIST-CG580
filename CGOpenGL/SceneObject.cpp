#include "SceneObject.h"

#include "AppManager.h"
#include "Debug.h"
#include "Geometry.h"
#include "ObjectManager.h"
#include "ShaderProgram.h"

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(Geometry* geometry, const ShaderProgram* shader) : geometry(geometry), shader(shader)
{
}


SceneObject::~SceneObject()
{
	AppManager::GetObjectManager()->UnSubscribeUpdate( this );
	AppManager::GetObjectManager()->UnSubscribeRender( this );

	SDELETE( geometry );
}

void SceneObject::Update()
{
	// Empty default update
}

void SceneObject::Render()
{
	// Bind the shader
	shader->BindShader();

	// Bind the uniforms
	glm::mat4 rot = glm::mat4_cast( rotation );
	glm::mat4 worldMatrix = glm::translate( glm::mat4(1.0f), position ) * 
		glm::mat4_cast( rotation ) * 
		glm::scale( glm::mat4(1.0f), scale );
	glUniformMatrix4fv( uniWorldMatrix, 1, false, &worldMatrix[0][0] );

	// Draw
	geometry->Draw( shader->GetBindSlots() );
}
