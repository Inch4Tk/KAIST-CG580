#include "SceneObject.h"

#include "Geometry.h"
#include "ShaderProgram.h"

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(Geometry* geometry, ShaderProgram* shader) : geometry(geometry), shader(shader)
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
