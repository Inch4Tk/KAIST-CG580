#include "Material.h"


Material::Material( glm::vec3 diffuse, glm::vec3 specular, glm::vec3 ambient, float shininess ) :
diffuse(diffuse), specular(specular), ambient(ambient), shininess(shininess)
{
	ShaderMaterial_Std140 sm;
	sm.diffuse = diffuse;
	sm.specular = specular;
	sm.ambient = ambient;
	sm.shininess = shininess;
	materialBuffer = new GLBuffer<ShaderMaterial_Std140>(1, &sm);
}

Material::~Material()
{
	SDELETE( materialBuffer );
}

/// <summary>
/// Binds the material to the uniform buffer and texture slots.
/// </summary>
/// <param name="slot">The slot.</param>
void Material::Bind( const std::unordered_map<std::string, uint32_t>& bindSlots )
{
	auto it = bindSlots.find( "UniMaterial" );
	if( it != bindSlots.end() )
	{
		materialBuffer->BindSlot( GL_UNIFORM_BUFFER, it->second );
	}
}
