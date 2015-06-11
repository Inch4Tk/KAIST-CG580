#version 430 core

const uint LIGHTS_LIMIT = 10;

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
layout( std140 ) uniform UniMaterial
{
	vec3 mat_diffuse;
	vec3 mat_specular;
	vec3 mat_ambient;
	float mat_shininess;
};

struct LightSource
{ // for now restrict ourselves to point lights
	vec3 position;
	float pad0;
	vec3 color;
	float range;
};
layout (std140) uniform UniLights {
	LightSource lights[LIGHTS_LIMIT];
	// Little bit of a hack, we actually reserve one more light space and can pack associated vars
	uint lightCount; 
};

// Light source as a point light (without attenuation)
// Pos and normal are supposed to be in world space
// Lights position is treated as world space
void PointLightPhongLighting( in LightSource light, in vec3 pos, in vec3 normal, inout vec3 diffuse, inout vec3 specular )
{
	float Kd;
	float Ks;

	// Renormalize the normal
	vec3 n = normalize( normal );

	// Calculate distance and attenuation
	vec3 toLightUnnorm = light.position - pos;
	float att = 1.0;
	if( light.range > 0 )
	{
		float dist = length( toLightUnnorm );
		att = clamp( 1.0 - dist / light.range, 0.0, 1.0 );
		att *= att;
	}
	// Calculate diffuse part
	vec3 toLight = normalize( toLightUnnorm );
	Kd = clamp( dot( n, toLight ), 0.0, 1.0 );
	diffuse += Kd * light.color * att;

	// Calculate specular part
	vec3 reflected = normalize( reflect( -toLight, n ) ); // Make the light direction incident
	vec3 posToView = normalize( worldCamPos - pos );
	if( Kd == 0.0 )
		Ks = 0.0;
	else
		Ks = pow( clamp( dot( posToView, reflected ), 0.0f, 1.0f ), mat_shininess ); // Insert material shininess here
	specular += Ks * light.color * att;
}

in VertexData{
	vec3 worldPos;
	vec3 normal;
} FragmentIn;

out vec4 color;

void main()
{
	// Lighting params
	vec3 dif = vec3( 0.0, 0.0, 0.0 );
	vec3 spec = vec3( 0.0, 0.0, 0.0 );

	// Simulate a point light with phong lighting
	for( uint i = 0; i < lightCount && i < LIGHTS_LIMIT; ++i )
	{
		PointLightPhongLighting( lights[i], FragmentIn.worldPos, normalize( FragmentIn.normal ), dif, spec );
	}

	color = vec4( clamp( dif * mat_diffuse + spec * mat_specular + ambient * mat_ambient, 0, 1 ), 1 );
}