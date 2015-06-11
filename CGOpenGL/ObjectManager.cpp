#include "ObjectManager.h"

#include "AppManager.h"
#include "Camera.h"
#include "Debug.h"
#include "Light.h"
#include "Scene.h"
#include "SceneObject.h"

ObjectManager::ObjectManager()
{
	ShaderGlobals_Std140 o;
	memset( &o, 0, sizeof( ShaderGlobals_Std140 ) );
	uniBufferGlobals = new GLBuffer<ShaderGlobals_Std140>( 1, &o );

	ShaderLight_Std140 l[CONFIG_MAX_LIGHTS + 1]; // Hack for packing more vars, we use maxlights + 1
	memset( l, 0, sizeof( ShaderLight_Std140 ) * (CONFIG_MAX_LIGHTS + 1) );
	uniBufferLights = new GLBuffer<ShaderLight_Std140>( CONFIG_MAX_LIGHTS + 1, l );
}


ObjectManager::~ObjectManager()
{
	SDELETE( uniBufferGlobals );
	SDELETE( uniBufferLights );
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
	// Update the global uniform buffer
	const Camera* cam = AppManager::GetScene()->GetActiveCamera();
	ShaderGlobals_Std140 o;
	o.viewMatrix = cam->GetView();
	o.projectionMatrix = cam->GetProjection();
	o.viewProjectionMatrix = cam->GetViewProjection();
	o.worldCamPos = cam->position;
	o.ambient = glm::vec3( 0.1f, 0.1f, 0.1f );
	o.worldUp = glm::vec3( 0, 1, 0 );
	o.invNear = 1.0f / cam->GetNearPlane();
	auto winDim = AppManager::GetWindowDimensions();
	o.invLogSubDiv = 2.0f * tanf( cam->GetFOV() * 0.5f ) / float( (winDim.second + AMT_TILES_Y - 1) / AMT_TILES_Y );
	uniBufferGlobals->CopyFromHost(&o, 1);

	// Update the lights uniform buffer
	std::vector<ShaderLight_Std140> ls;
	uint32_t numLights = 0;
	for( Light* l : sceneLights )
	{
		if( !l->IsActive() )
			continue;
		ls.emplace_back( l->position, l->GetColor(), l->GetRange() );
		++numLights;
	}
	ShaderLight_Std140* target = uniBufferLights->BeginMapWrite();
	memcpy( target, &ls[0], ls.size() * sizeof( ShaderLight_Std140 ) );
	memcpy( target + CONFIG_MAX_LIGHTS, &numLights, sizeof( uint32_t ) );
	uniBufferLights->EndMap();

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
/// Adds the light.
/// </summary>
/// <param name="light">The light.</param>
void ObjectManager::AddLight( Light* light )
{
	sceneLights.push_back( light );
}

/// <summary>
/// Unsubscribes the sub from update.
/// </summary>
/// <param name="sub">The sub.</param>
void ObjectManager::UnSubscribeUpdate( SceneObject* sub )
{
	subsUpdate.remove( sub );
}

/// <summary>
/// Unsubscribes the sub from render.
/// </summary>
/// <param name="sub">The sub.</param>
void ObjectManager::UnSubscribeRender( SceneObject* sub )
{
	subsRender.remove( sub );
}

/// <summary>
/// Removes the light.
/// </summary>
/// <param name="light">The light.</param>
void ObjectManager::RemoveLight( Light* light )
{
	sceneLights.remove( light );
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
	// Lights
	it = uniSlots.find( "UniLights" );
	if( it != uniSlots.end() )
	{
		uniBufferLights->BindSlot( GL_UNIFORM_BUFFER, it->second );
	}
}


