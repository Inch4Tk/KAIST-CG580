#version 430 core
#extension GL_EXT_shader_image_load_store : enable

const uint AMT_TILES_X = 64;
const uint AMT_TILES_Y = 64;

layout( std140 ) uniform UniGlobals
{
	mat4 worldMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
	vec3 ambient;
	vec3 worldUp;
	vec3 worldCamPos;
	float invNear; // 1.0f / nearPlane
	float invLogSubDiv; // 1.0f / log(subDivision + 1.0f); subDivision = 2.0f * tan( fov * 0.5f ) / gridDimY;
};

// Which clusters actually exist uffer (DX: UAV)
layout( binding = 0, size1x32 ) uniform uimageBuffer usedClusters;

in VertexData{
	vec3 viewPos;
} FragmentIn;

out vec4 color;

// Determine the cluster offset in the usedClusters UAV.
// Determined after the algorithm provided by:
// http://www.cse.chalmers.se/~olaolss/main_frame.php?contents=publication&id=clustered_shading
uint getClusterOffset( vec2 screenXY, float depth )
{
	uint clusterIdX = uint( screenXY.x ) / AMT_TILES_X;
	uint clusterIdY = uint( screenXY.y ) / AMT_TILES_Y;
	uint clusterIdZ = uint( log( -depth * invNear ) * invLogSubDiv );

	return clusterIdX + clusterIdY * AMT_TILES_X + clusterIdZ * AMT_TILES_X * AMT_TILES_Y;
}

void main()
{
	// Write to the cluster that we are in
	imageStore( usedClusters, int( getClusterOffset( gl_FragCoord.xy, FragmentIn.viewPos.z ) ), uvec4( 1U ) );
	color = vec4( 1.0 );
}