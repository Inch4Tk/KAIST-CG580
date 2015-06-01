#include "SceneObject.h"

#include "Geometry.h"
#include "ShaderProgram.h"

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(Geometry* geometry, const ShaderProgram* shader) : geometry(geometry), shader(shader)
{
	// Set the uniform locations
	uniWorldMatrix = shader->GetUniformLocation( "worldMatrix" );
}


SceneObject::~SceneObject()
{
	SDELETE( geometry );
}

void SceneObject::Update()
{

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

	// Draw
	geometry->Draw();
}
