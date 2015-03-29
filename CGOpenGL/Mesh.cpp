#include "Mesh.h"

#include "GLM/glm.hpp"
#include "GL/glew.h"

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
	glDeleteBuffers( 1, &vboID );
	glDeleteVertexArrays( 1, &vaoID );
}

/// <summary>
/// Initializes the vertex array object with the specified format and data.
/// </summary>
/// <param name="format">The format.</param>
/// <param name="data">The data.</param>
/// <param name="datasize">The datasize.</param>
/// <param name="primitiveType">Type of the primitive.</param>
/// <returns></returns>
int Mesh::Initialize( const VertexFormat& format, void* data, uint32_t datasize, GLenum primitiveType )
{
	// Create vertex array object
	glGenVertexArrays( 1, &vaoID );
	glBindVertexArray( vaoID );
	glEnableVertexAttribArray( 0 );

	// Create vertex buffer object and push data
	glGenBuffers( 1, &vboID );
	glBindVertexBuffer( 0, vboID, 0, format.bytesize );

	glBufferData( 0, datasize, data, GL_STATIC_DRAW );

	// Specify the buffer format
	vbFormat = format;
	for( uint32_t i = 0; i < format.sizes.size(); ++i )
	{
		if( format.asInt[i] )
			glVertexAttribIFormat( i, format.sizes[i], format.types[i], format.offsets[i] );
		else
			glVertexAttribFormat( i, format.sizes[i], format.types[i], format.normalized[i], format.offsets[i] );
		glVertexAttribBinding( i, 0 );
	}
	initialized = true;
}

/// <summary>
/// Binds the mesh for drawing, then draws the mesh
/// </summary>
const void Mesh::Draw()
{
	if( !initialized )
		return;

	glBindVertexArray( vaoID );
	glDrawArrays( primitiveType, 0, numVertices );
}
