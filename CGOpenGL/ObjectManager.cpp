#include "ObjectManager.h"

#include "AppManager.h"
#include "Camera.h"
#include "Debug.h"
#include "Input.h"
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
	uniTexBufUsedClusters = new GLBuffer<uint32_t>( Config::AMT_TILES_X * Config::AMT_TILES_Y * Config::AMT_TILES_Z );
	uint32_t* p = uniTexBufUsedClusters->BeginMapWrite();
	memset( p, 0, sizeof( uint32_t ) * uniTexBufUsedClusters->Size() );
	uniTexBufUsedClusters->EndMap();
	uniTexBufUsedClusters->MakeTexBuffer( GL_R32UI );

	uniTexBufClusters = new GLBuffer<glm::uvec2>( Config::AMT_TILES_X * Config::AMT_TILES_Y * Config::AMT_TILES_Z );
	uniTexBufClusters->MakeTexBuffer( GL_RG32UI );

	uniTexBufClusterLightIdx = new GLBuffer<int32_t>( 1 );
	uniTexBufClusterLightIdx->MakeTexBuffer( GL_R32I );

	// Set up a hook for swapping clustering
	AppManager::GetInput()->RegisterKeyEventHook( GLFW_KEY_F2, GLFW_PRESS, [&]()
	{
		clusteringActive = !clusteringActive;
		Debug::Log( "Clustering is active: " + std::to_string( clusteringActive ) );
	} );
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
	o.invLogSubDiv = 1.0f / (2.0f * tanf( cam->GetFOV() * 0.5f ) / static_cast<float>( Config::AMT_TILES_Y ));
	o.amtTilesX = Config::AMT_TILES_X;
	o.amtTilesY = Config::AMT_TILES_Y;
	o.dimTilesX = Config::DIM_TILES_X;
	o.dimTilesY = Config::DIM_TILES_Y;
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
	if( numLights > 0 )
	{
		memcpy( target, &ls[0], ls.size() * sizeof( ShaderLight_Std140 ) );
	}
	memcpy( target + CONFIG_MAX_LIGHTS, &numLights, sizeof( uint32_t ) );
	uniBufferLights->EndMap();

	// Perform clustering
	if( clusteringActive )
	{
		glDepthFunc( GL_LEQUAL ); // This is important, so we actually render something in the second pass

		// Cluster-madness
		BuildCluster( o.invNear, o.invLogSubDiv );
		
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
void ObjectManager::BuildCluster( float invNear, float invLogSubDiv )
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
		CalcClusterCPU( invNear, invLogSubDiv );
	}
}

/// <summary>
/// Calculates the cluster on the cpu.
/// </summary>
void ObjectManager::CalcClusterCPU( float invNear, float invLogSubDiv )
{
	Camera* mainCam = AppManager::GetScene()->GetActiveCamera();
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

#define INDEX(X,Y,Z) ( X + Config::AMT_TILES_X * ( y + ( z * Config::AMT_TILES_Y ) ) )

	uint32_t totalClusteredLights = 0;

	// The way the algorithm right now is structured it has to walk over lights 2 times, and over the clusters 1 times.
	// Whereas is you just run over all the lights for every cluster it would be lights * clusters

	// Write the amount of lights for every cluster
	for( Light* l : sceneLights )
	{
		if( !l->IsActive() )
			continue;

		std::pair<glm::uvec3, glm::uvec3> cluster = l->GetClusterExtents( mainCam, invNear, invLogSubDiv );

		// Go over all the clusters affected by the light
		for( uint32_t z = cluster.first.z; z < cluster.second.z; ++z )
		{
			for( uint32_t y = cluster.first.y; y < cluster.second.y; ++y )
			{
				for( uint32_t x = cluster.first.x; x < cluster.second.x; ++x )
				{
					// Add a light to the cluster if it is occupied
					uint32_t idx = INDEX( x, y, z );
					if( usedClusters[idx] > 0 )
					{
						clusters[idx].y += 1;
						++totalClusteredLights;
					}
				}
			}
		}
	}

	// Go over all clusters
	uint32_t offset = 0;
	uint32_t occupiedClusters = 0;
	if( totalClusteredLights > 0 )
	{
		for( uint32_t z = 0; z < Config::AMT_TILES_Z; ++z )
		{
			for( uint32_t y = 0; y < Config::AMT_TILES_Y; ++y )
			{
				for( uint32_t x = 0; x < Config::AMT_TILES_X; ++x )
				{
					// Set the amount as the new offset and
					uint32_t idx = INDEX( x, y, z );
					uint32_t count = clusters[idx].y;
					clusters[idx].x = offset;
					clusters[idx].y = 0;
					offset += count;
					// Debug info
					if( usedClusters[idx] )
					{
						++occupiedClusters;
					}
				}
			}
		}
	}
	

	// Go over the lights and assign them to the lights list
	lightIndicesPerCluster.resize( totalClusteredLights );
	uint32_t lIdx = 0;
	for( Light* l : sceneLights )
	{
		if( !l->IsActive() )
			continue;

		std::pair<glm::uvec3, glm::uvec3> cluster = l->GetClusterExtents( mainCam, invNear, invLogSubDiv );

		// Go over all the clusters affected by the light
		for( uint32_t z = cluster.first.z; z < cluster.second.z; ++z )
		{
			for( uint32_t y = cluster.first.y; y < cluster.second.y; ++y )
			{
				for( uint32_t x = cluster.first.x; x < cluster.second.x; ++x )
				{
					// Add a light to the cluster if it is occupied
					uint32_t idx = INDEX( x, y, z );
					if( usedClusters[idx] > 0 )
					{
						lightIndicesPerCluster[clusters[idx].x + clusters[idx].y] = lIdx;
						clusters[idx].y += 1;
					}
				}
			}
		}
		++lIdx;
	}

#undef INDEX
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


