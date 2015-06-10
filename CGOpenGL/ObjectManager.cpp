#include "ObjectManager.h"

#include "AppManager.h"
#include "Camera.h"
#include "Debug.h"
#include "Scene.h"
#include "SceneObject.h"

ObjectManager::ObjectManager()
{
	ShaderGlobals_Std140 o;
	memset( &o, 0, sizeof( ShaderGlobals_Std140 ) );
	uniBufferGlobals = new GLBuffer<ShaderGlobals_Std140>( 1, &o );
}


ObjectManager::~ObjectManager()
{
	SDELETE( uniBufferGlobals );
}

/// <summary>
/// Executes the update calls on all subscribed objects.
/// </summary>
void ObjectManager::ExecUpdate()
{
	for( SceneObject* ro : subsUpdate )
		ro->Update();
}

/// <summary>
/// Executes the render calls on all subscribed objects.
/// </summary>
void ObjectManager::ExecRender()
{
	// Update the global uniform buffers
	const Camera* cam = AppManager::GetScene()->GetActiveCamera();
	ShaderGlobals_Std140 o;
	o.viewMatrix = cam->GetView();
	o.projectionMatrix = cam->GetProjection();
	o.viewProjectionMatrix = cam->GetViewProjection();
	o.worldCamPos = cam->position;
	o.ambient = glm::vec3( 0.1f, 0.1f, 0.1f );
	o.worldUp = glm::vec3( 0, 1, 0 );
	uniBufferGlobals->CopyFromHost(&o, 1);

	// Render the objects
	for( SceneObject* ro : subsRender )
		ro->Render();
}

/// <summary>
/// Subscribes sub to update calls.
/// </summary>
/// <param name="sub">The sub.</param>
void ObjectManager::SubscribeUpdate( SceneObject* sub )
{
	subsUpdate.push_back( sub );
}

/// <summary>
/// Subscribes sub to render calls.
/// </summary>
/// <param name="sub">The sub.</param>
void ObjectManager::SubscribeRender( SceneObject* sub )
{
	subsRender.push_back( sub );
}

/// <summary>
/// Binds all the known per frame uniform buffers, depending on the available slots in the shader
/// </summary>
/// <param name="uniSlots">The uni slots.</param>
void ObjectManager::BindPerFrameUniformBuffer( const std::unordered_map<std::string, uint32_t>& uniSlots )
{
	// Globals
	auto it = uniSlots.find( "UniGlobals" );
	if( it != uniSlots.end() )
	{
		uniBufferGlobals->BindSlot( GL_UNIFORM_BUFFER, it->second );
	}
}

