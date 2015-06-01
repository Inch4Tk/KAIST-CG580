#include "Mesh.h"

#include "GLM/glm.hpp"
#include "GL/glew.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	glDeleteBuffers( 1, &vboID );
	glDeleteBuffers( 1, &iboID );
	glDeleteVertexArrays( 1, &vaoID );
}

/// <summary>
/// Initializes the vertex array object with the specified format and data.
/// </summary>
/// <param name="format">The format.</param>
/// <param name="data">The data.</param>
/// <param name="datasize">The datasize in byte.</param>
/// <param name="primitiveType">Type of the primitive.</param>
/// <param name="material">The material.</param>
/// <returns></returns>
int Mesh::Initialize( const VertexFormat& format, void* data, uint32_t datasize, 
					  GLenum primitiveType, Material* material /*= nullptr*/ )
{
	// Take over primitive type
	this->primitiveType = primitiveType;

	// Create vertex array object
	glGenVertexArrays( 1, &vaoID );
	glBindVertexArray( vaoID );

	// Create vertex buffer object and push data
	glGenBuffers( 1, &vboID );
	glBindBuffer( GL_ARRAY_BUFFER, vboID );
	glBufferData( GL_ARRAY_BUFFER, datasize, data, GL_STATIC_DRAW );

	// Specify the buffer format
	glBindVertexBuffer( 0, vboID, 0, format.bytesize );
	numVertices = datasize / format.bytesize;
	vbFormat = format;
	for( uint32_t i = 0; i < format.sizes.size(); ++i )
	{
		glEnableVertexAttribArray( i );
		if( format.asInt[i] )
			glVertexAttribIFormat( i, format.sizes[i], format.types[i], format.offsets[i] );
		else
			glVertexAttribFormat( i, format.sizes[i], format.types[i], format.normalized[i], format.offsets[i] );
		glVertexAttribBinding( i, 0 );
	}

	glBindVertexArray( 0 );

	initialized = true;
	return 0;
}

/// <summary>
/// Initializes the vertex array object with the specified format and data with an index buffer.
/// </summary>
/// <param name="format">The format.</param>
/// <param name="vdata">The vdata.</param>
/// <param name="vdatasize">The vdatasize.</param>
/// <param name="indices">The indices.</param>
/// <param name="primitiveType">Type of the primitive.</param>
/// <param name="material">The material.</param>
/// <returns></returns>
int Mesh::Initialize( const VertexFormat& format, void* vdata, uint32_t vdatasize, const std::vector<uint32_t>& indices,
					  GLenum primitiveType, Material* material /*= nullptr */ )
{
	// Init index buffer
	glGenBuffers( 1, &iboID );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iboID );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof( uint32_t ), &indices[0], GL_STATIC_DRAW );
	numIndices = static_cast<uint32_t>(indices.size());
	indexed = true;
	// Init rest
	return Initialize( format, vdata, vdatasize, primitiveType, material );
}

/// <summary>
/// Binds the mesh for drawing, then draws the mesh
/// </summary>
void Mesh::Draw() const
{
	if( !initialized )
		return;

	if( indexed )
	{
		glBindVertexArray( vaoID );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, iboID );
		glDrawElements( primitiveType, numIndices, GL_UNSIGNED_INT, nullptr );
		glBindVertexArray( 0 );
	}
	else
	{
		glBindVertexArray( vaoID );
		glDrawArrays( primitiveType, 0, numVertices );
		glBindVertexArray( 0 );
	}
}
