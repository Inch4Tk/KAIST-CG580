#include "Geometry.h"

#include "Material.h"
#include "Mesh.h"
#include "Scene.h"

// Include tiny object loader module
#include "tiny_obj_loader.h"

Geometry::Geometry( const std::vector<Mesh*>& meshes ) : meshes( meshes )
{
}

// Load from filename
Geometry::Geometry( const std::string& objFilename )
{
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	tinyobj::LoadObj( shapes, materials, ("Resources/" + objFilename).c_str(), "Resources/" );

	// Run over all materials and create a corresponding material
	// Also temporarily safe references to the materials
	std::vector<Material*> tmp_mats;
	for( tinyobj::material_t mat : materials )
	{
		// Check if the material already exists
		std::string s = objFilename + mat.name;
		Material* m = Scene::GetMaterial( s );
		if( m != nullptr )
		{
			tmp_mats.push_back( m );
		}
		else
		{
			m = new Material();
			tmp_mats.push_back( m );
			Scene::RegisterMaterial( s, m );
		}
	}

	// Run over all shapes and create a corresponding mesh for every shape
	for( tinyobj::shape_t shape : shapes )
	{
		// Check if the mesh already exists
		std::string meshName = objFilename + shape.name;
		Mesh* m = Scene::GetMesh( meshName );
		if( m != nullptr )
		{
			meshes.push_back( m );
		}
		else
		{
			m = new Mesh();

			// Check what kind of data exists
			bool norm = false;
			bool tex = false;
			bool index = false;
			Material* mat = nullptr;
			if( shape.mesh.normals.size() > 0 )
				norm = true;
			if( shape.mesh.texcoords.size() > 0 )
				tex = true;
			if( shape.mesh.indices.size() > 0 )
				index = true;
			if( shape.mesh.material_ids.size() > 0 )
				mat = tmp_mats[shape.mesh.material_ids[0]];

			// Create and average normals
			std::vector<glm::vec3> tmp_norms;
			if( !norm )
			{
				// Create a real vertex array
				std::vector<glm::vec3> tmp_verts;
				for( uint32_t i = 0; i < shape.mesh.positions.size() / 3; ++i )
				{
					tmp_verts.push_back( glm::vec3( shape.mesh.positions[i],
						shape.mesh.positions[i + 1], shape.mesh.positions[i + 2] ) );
				}
				// If its indexed, normal calculation is a bit more complicated
				// because we need to find shared faces and average
				if( index )
				{
					tmp_norms.resize( tmp_verts.size() );
					// Create a normal for every face and add to the vertex normals
					for( uint32_t i = 0; i < shape.mesh.indices.size(); i += 3 )
					{
						glm::vec3 norm = glm::normalize( glm::cross(
							tmp_verts[shape.mesh.indices[i + 1]] - tmp_verts[shape.mesh.indices[i]],
							tmp_verts[shape.mesh.indices[i + 2]] - tmp_verts[shape.mesh.indices[i]] ) );
						tmp_norms[shape.mesh.indices[i]] += norm;
						tmp_norms[shape.mesh.indices[i + 1]] += norm;
						tmp_norms[shape.mesh.indices[i + 2]] += norm;
					}
					// Normalize over all vertex normals
					for( uint32_t i = 0; i < tmp_norms.size(); ++i )
					{
						tmp_norms[i] = glm::normalize( tmp_norms[i] );
					}
				}
				else
				{
					// Create normals for every 3 vertices = 1 face
					for( uint32_t i = 0; i < tmp_verts.size(); i += 3 )
					{
						glm::vec3 norm = glm::normalize( glm::cross( tmp_verts[i + 1] - tmp_verts[i],
							tmp_verts[i + 2] - tmp_verts[i] ) );
						tmp_norms.push_back( norm );
						tmp_norms.push_back( norm );
						tmp_norms.push_back( norm );
					}
				}
			}

			// Create a interleaved memory representation
			std::vector<float> mem;
			for( uint32_t i = 0; i < shape.mesh.positions.size() / 3; ++i )
			{
				mem.push_back( shape.mesh.positions[3 * i] );
				mem.push_back( shape.mesh.positions[3 * i + 1] );
				mem.push_back( shape.mesh.positions[3 * i + 2] );
				if( norm ) // Load normals
				{
					mem.push_back( shape.mesh.normals[3 * i] );
					mem.push_back( shape.mesh.normals[3 * i + 1] );
					mem.push_back( shape.mesh.normals[3 * i + 2] );
				}
				else // Use created normals
				{
					mem.push_back( tmp_norms[i].x );
					mem.push_back( tmp_norms[i].y );
					mem.push_back( tmp_norms[i].z );
				}
				if( tex )
				{
					mem.push_back( shape.mesh.texcoords[2 * i] );
					mem.push_back( shape.mesh.texcoords[2 * i + 1] );
				}
			}

			// Init the mesh
			if( index && tex )
			{

				m->Initialize( VertexFormatManager::Get3F3F2F(), &mem[0], static_cast<uint32_t>(mem.size() * 4),
							   shape.mesh.indices, GL_TRIANGLES, mat );
			}
			else if( index )
			{
				m->Initialize( VertexFormatManager::Get3F3F(), &mem[0], static_cast<uint32_t>(mem.size() * 4),
							   shape.mesh.indices, GL_TRIANGLES, mat );
			}
			else if( tex )
			{
				m->Initialize( VertexFormatManager::Get3F3F2F(), &mem[0], static_cast<uint32_t>(mem.size() * 4),
							   GL_TRIANGLES, mat );
			}
			else{
				m->Initialize( VertexFormatManager::Get3F3F(), &mem[0], static_cast<uint32_t>(mem.size() * 4), 
					GL_TRIANGLES, mat);
			}

			meshes.push_back( m );
			Scene::RegisterMesh( meshName, m );
		}
	}
}

Geometry::~Geometry()
{
}


/// <summary>
/// Runs over all meshes belonging to the geometry
/// </summary>
void Geometry::Draw() const
{
	for( Mesh* m : meshes )
		m->Draw();
}
