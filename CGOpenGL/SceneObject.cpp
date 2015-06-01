#include "SceneObject.h"

#include "Geometry.h"
#include "ShaderProgram.h"

SceneObject::SceneObject()
{
}

SceneObject::SceneObject(Geometry* geometry, const ShaderProgram* shader) : geometry(geometry), shader(shader)
{
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
	shader->BindShader();
	geometry->Draw();
}
