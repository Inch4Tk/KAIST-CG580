#version 430 core
#extension GL_EXT_shader_image_load_store : enable

layout( std140 ) uniform UniGlobals
{
	mat4 worldMatrix;
	mat4 viewMatrix;
	mat4 projectionMatrix;
	mat4 viewProjectionMatrix;
	vec3 ambient;
	float invLogSubDiv; // 1.0f / log(subDivision + 1.0f); subDivision = 2.0f * tan( fov * 0.5f ) / gridDimY;
	vec3 worldUp;
	float invNear; // 1.0f / nearPlane
	vec3 worldCamPos;
	uint dimTilesX;
	uint dimTilesY;
	uint amtTilesX;
	uint amtTilesY;
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
	uint clusterIdX = uint( screenXY.x ) / dimTilesX;
	uint clusterIdY = uint( screenXY.y ) / dimTilesY;
	uint clusterIdZ = uint( log( -depth * invNear ) * invLogSubDiv );

	return clusterIdX + amtTilesX * (clusterIdY + clusterIdZ * amtTilesY);
}

void main()
{
	// Write to the cluster that we are in
	imageStore( usedClusters, int( getClusterOffset( gl_FragCoord.xy, FragmentIn.viewPos.z ) ), uvec4( 1U ) );
	color = vec4( 1.0 );
}