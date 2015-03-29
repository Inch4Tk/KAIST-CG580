#include "SceneObject.h"

#include "Mesh.h"
#include "ShaderProgram.h"

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(Mesh* geometry, ShaderProgram* shader) : geometry(geometry), shader(shader)
{

}


SceneObject::~SceneObject()
{
}

void SceneObject::Update()
{

}

void SceneObject::Render()
{
	shader->BindShader();
	geometry->Draw();
}
