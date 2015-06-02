#include "SceneObject.h"

#include "Camera.h"
#include "Geometry.h"
#include "Scene.h"
#include "ShaderProgram.h"

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(Geometry* geometry, const ShaderProgram* shader) : geometry(geometry), shader(shader)
{
	// Set the uniform locations
	uniWorldMatrix = shader->GetUniformLocation( "worldMatrix" );
	uniViewMatrix = shader->GetUniformLocation( "viewMatrix" );
	uniProjectionMatrix = shader->GetUniformLocation( "projectionMatrix" );
	uniViewProjectionMatrix = shader->GetUniformLocation( "viewProjectionMatrix" );
}


SceneObject::~SceneObject()
{
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
	glm::mat4 worldMatrix = glm::translate( glm::mat4(1.0f), position ) * 
		glm::mat4_cast( rotation ) * 
		glm::scale( glm::mat4(1.0f), scale );
	glUniformMatrix4fv( uniWorldMatrix, 1, false, &worldMatrix[0][0] );
	const Camera* cam = Scene::GetActiveCamera();
	glUniformMatrix4fv( uniViewMatrix, 1, false, &cam->GetView()[0][0] );
	glUniformMatrix4fv( uniProjectionMatrix, 1, false, &cam->GetProjection()[0][0] );
	glUniformMatrix4fv( uniViewProjectionMatrix, 1, false, &cam->GetViewProjection()[0][0] );

	// Draw
	geometry->Draw();
}
