#include "ObjectManager.h"

#include "AppManager.h"
#include "Camera.h"
#include "Debug.h"
#include "Light.h"
#include "Scene.h"
#include "SceneObject.h"

ObjectManager::ObjectManager()
{
	// Globals and lights uniform buffers
	ShaderGlobals_Std140 o;
	memset( &o, 0, sizeof( ShaderGlobals_Std140 ) );
	uniBufferGlobals = new GLBuffer<ShaderGlobals_Std140>( 1, &o );

	ShaderLight_Std140 l[CONFIG_MAX_LIGHTS + 1]; // Hack for packing more vars, we use maxlights + 1
	memset( l, 0, sizeof( ShaderLight_Std140 ) * (CONFIG_MAX_LIGHTS + 1) );
	uniBufferLights = new GLBuffer<ShaderLight_Std140>( CONFIG_MAX_LIGHTS + 1, l );

	// Texture buffers
	uniTexBufUsedClusters = new GLBuffer<uint32_t>( AMT_TILES_X * AMT_TILES_Y * MAX_TILES_Z );
	uint32_t* p = uniTexBufUsedClusters->BeginMapWrite();
	memset( p, 0, sizeof( uint32_t ) * uniTexBufUsedClusters->Size() );
	uniTexBufUsedClusters->EndMap();
	uniTexBufUsedClusters->MakeTexBuffer( GL_R32UI );

	uniTexBufClusters = new GLBuffer<glm::uvec2>( AMT_TILES_X * AMT_TILES_Y * MAX_TILES_Z );
	uniTexBufClusters->MakeTexBuffer( GL_RG32UI );

	uniTexBufClusterLightIdx = new GLBuffer<int32_t>( 1 );
	uniTexBufClusterLightIdx->MakeTexBuffer( GL_R32I );
}


ObjectManager::~ObjectManager()
{
	SDELETE( uniBufferGlobals );
	SDELETE( uniBufferLights );
	SDELETE( uniTexBufUsedClusters );
	SDELETE( uniTexBufClusters );
	SDELETE( uniTexBufClusterLightIdx );
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

	// Perform clustering
	if( clusteringActive )
	{
		glDepthFunc( GL_LEQUAL ); // This is important, so we actually render something in the second pass

		// Cluster-madness
		BuildCluster();
		
		// Bind the cluster infos
		uniTexBufClusters->BindTexture( 0 );
		uniTexBufClusterLightIdx->BindTexture( 1 );
		CHECK_GL_ERROR();

		// Render the objects 
		for( SceneObject* ro : subsRender )
			ro->Render( "clusterPhong" );
	}
	else
	{
		// Render the objects
		for( SceneObject* ro : subsRender )
			ro->Render( "phong" );
	}
	
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

/// <summary>
/// Builds the cluster.
/// </summary>
void ObjectManager::BuildCluster()
{
	// Unbind the textures in case
	uniTexBufClusters->UnbindTexture();
	uniTexBufClusterLightIdx->UnbindTexture();

	// Render the objects to check for clusters (No color output)
	uniTexBufUsedClusters->BindTexture( 0, true );
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	for( SceneObject* ro : subsRender )
		ro->Render( "clusterPre" );
	glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
	uniTexBufUsedClusters->UnbindTexture();
	CHECK_GL_ERROR();

	// Perform clustering
	if( cudaClustering )
	{

	}
	else
	{
		CalcClusterCPU();
	}
}

/// <summary>
/// Calculates the cluster on the cpu.
/// </summary>
void ObjectManager::CalcClusterCPU()
{
	// Vector of light indices per cluster
	std::vector<glm::uvec2> clusters; // Offset + Amount for every cluster
	clusters.resize(uniTexBufClusters->Size());
	std::vector<int32_t> lightIndicesPerCluster; // These are accessed by offset and amount of lights

	// First get the used clusters
	std::vector<uint32_t> usedClusters(uniTexBufUsedClusters->Size());
	uniTexBufUsedClusters->CopyToHost(&usedClusters[0], uniTexBufUsedClusters->Size());
	// Then reset the used clusters on GPU
	uint32_t* p = uniTexBufUsedClusters->BeginMapWrite();
	memset( p, 0, sizeof( uint32_t ) * uniTexBufUsedClusters->Size() );
	uniTexBufUsedClusters->EndMap();

	// Run over all clusters, if the cluster is used, we determine lights and offset
	// This is quite inefficient compared to first building rects around lights, 
	// and then only testing for clusters that are close. But quick to implement.
	for( uint32_t i = 0; i < usedClusters.size(); ++i )
	{
		// Only if the cluster actually contains anything
		if( usedClusters[i] > 0 )
		{
			// Go over lights and check for overlaps
			uint32_t offset = static_cast<uint32_t>(lightIndicesPerCluster.size());
			uint32_t assignedLights = 0;
			for( size_t l = 0; l < sceneLights.size(); ++l )
			{
				// For now we assign every light to every cluster, maximal inefficiency
				lightIndicesPerCluster.push_back( static_cast<int>(l) );

				++assignedLights;
			}
			clusters[i] = glm::uvec2( offset, assignedLights );
		}
	}

	// Shove data to GPU
	uniTexBufClusters->CopyFromHost( &clusters[0], clusters.size() );
	if( lightIndicesPerCluster.size() > 0 )
	{
		uniTexBufClusterLightIdx->CopyFromHost( &lightIndicesPerCluster[0], lightIndicesPerCluster.size() );
	}
	else
	{
		uniTexBufClusterLightIdx->CopyFromHost( nullptr, 1 );
	}
	CHECK_GL_ERROR();
}


