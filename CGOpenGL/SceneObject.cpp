#include "SceneObject.h"

#include "Mesh.h"
#include "ShaderProgram.h"

SceneObject::SceneObject()
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
