#pragma once

#include "StandardIncludes.h"
#include "Debug.h"

template <typename T>
class GLBuffer
{
public:
	GLBuffer(size_t elementCount, const T* data = nullptr, uint32_t updateMethod = GL_DYNAMIC_COPY) : 
		elementCount(elementCount), updateMethod(updateMethod)
	{
		glGenBuffers( 1, &id );
		CHECK_GL_ERROR();

		if( elementCount )
		{
			CopyFromHost( data, elementCount );
		}
	};
	~GLBuffer()
	{
		if( texId )
		{
			glDeleteTextures( 1, &texId );
		}
		if( id )
		{
			glDeleteBuffers( 1, &id );
		}	
	};

	/// <summary>
	/// Resizes the buffer.
	/// </summary>
	/// <param name="elementCount">The element count.</param>
	void Resize( size_t elementCount )
	{
		CopyFromHost( 0, elementCount );
	}

	/// <summary>
	/// Gets the element count.
	/// </summary>
	/// <returns></returns>
	size_t Size() const
	{
		return elementCount;
	}

	/// <summary>
	/// Makes the buffer a texture buffer.
	/// </summary>
	/// <param name="type">The type.</param>
	void MakeTexBuffer( GLenum type )
	{
		texType = type;
		glGenTextures( 1, &texId );
		glBindTexture( GL_TEXTURE_BUFFER, texId );
		glTexBuffer( GL_TEXTURE_BUFFER, type, id );
		glBindTexture( GL_TEXTURE_BUFFER, 0 );
		CHECK_GL_ERROR();
	}

	/// <summary>
	/// Copies data from host.
	/// </summary>
	/// <param name="data">The data.</param>
	/// <param name="elementCount">The element count.</param>
	void CopyFromHost( const T* data, size_t elementCount )
	{
		ASSERT( elementCount > 0 );
		this->elementCount = elementCount;
		glBindBuffer( GL_ARRAY_BUFFER, id );

		// Push buffer data
		glBufferData( GL_ARRAY_BUFFER, elementCount * sizeof( T ), data, updateMethod );
		CHECK_GL_ERROR();

		// Unbind
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
	}

	/// <summary>
	/// Copies buffer data to host.
	/// </summary>
	/// <param name="data">The data.</param>
	/// <param name="count">The count.</param>
	void CopyToHost( T* data, size_t count = 0 )
	{
		ASSERT( elementCount > 0 );
		memcpy( data, BeginMapRead(), std::min( count, elementCount() ) * sizeof( T ) );
		EndMap();
	}

	/// <summary>
	/// Maps the buffer for reading
	/// </summary>
	/// <returns></returns>
	const T* BeginMapRead() const
	{
		ASSERT( elementCount > 0 );
		glBindBuffer( GL_ARRAY_BUFFER, id );
		const T* buffer = reinterpret_cast<const T*>(glMapBuffer( GL_ARRAY_BUFFER, GL_READ_ONLY ));
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		return buffer;
	}

	/// <summary>
	/// Maps the buffer for writing
	/// </summary>
	/// <param name="readWrite">if set to <c>true</c> [read write].</param>
	/// <returns></returns>
	T* BeginMapWrite( bool readWrite = false )
	{
		ASSERT( elementCount > 0 );
		glBindBuffer( GL_ARRAY_BUFFER, id );

		T* result = reinterpret_cast<T*>(glMapBuffer( GL_ARRAY_BUFFER, readWrite ? GL_READ_WRITE : GL_WRITE_ONLY ));
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		return result;
	}

	/// <summary>
	/// Unmaps the buffer
	/// </summary>
	void EndMap() const
	{
		glBindBuffer( GL_ARRAY_BUFFER, id );
		glUnmapBuffer( GL_ARRAY_BUFFER );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		CHECK_GL_ERROR();
	}

	/// <summary>
	/// Binds the buffer to the specified target
	/// </summary>
	/// <param name="target">The target.</param>
	void Bind( GLenum target = GL_ARRAY_BUFFER ) const
	{
		glBindBuffer( target, id );
		CHECK_GL_ERROR();
	}

	/// <summary>
	/// Binds the buffer to a specified index/slot.
	/// </summary>
	/// <param name="target">The target.</param>
	/// <param name="slot">The slot.</param>
	void BindSlot( GLenum target, uint32_t slot ) const
	{
		glBindBufferBase( target, slot, id );
		CHECK_GL_ERROR();
	}

	/// <summary>
	/// Binds the buffer texture to a texture unit. Or alternatively as a UAV to get written into. (Then texunit = index)
	/// </summary>
	/// <param name="texUnit">The tex unit.</param>
	/// <param name="write">if set to <c>true</c> [write].</param>
	void BindTexture( int texUnit, bool write = false )
	{
		if( texBound )
			UnbindTexture();
		if( write )
		{
			glBindImageTextureEXT( texUnit, texId, 0, false, 0, GL_WRITE_ONLY, texType );
			texWriteBound = true;
		}
		else
		{
			this->texUnit = texUnit;
			glActiveTexture( GL_TEXTURE0 + texUnit );
			glBindTexture( texType, texId );
		}
		texBound = true;
	}

	/// <summary>
	/// Unbinds the buffer from the specified target.
	/// </summary>
	/// <param name="target">The target.</param>
	void Unbind( GLenum target = GL_ARRAY_BUFFER ) const
	{
		glBindBuffer( target, 0 );
	}

	/// <summary>
	/// Unbinds the texture.
	/// </summary>
	void UnbindTexture()
	{
		if( texBound && !texWriteBound )
		{
			glActiveTexture( GL_TEXTURE0 + texUnit );
			glBindTexture( texType, 0 );
			texBound = false;
		}
		else if( texBound && texWriteBound )
		{
			glBindImageTextureEXT( texUnit, 0, 0, false, 0, GL_WRITE_ONLY, texType );
			texBound = false;
			texWriteBound = false;
		}
	}

private:
	size_t elementCount = 0;
	GLuint id = 0;
	GLuint updateMethod = GL_DYNAMIC_COPY;
	// Texture
	GLuint texId = 0;
	GLenum texType = 0;
	int texUnit = 0;
	bool texBound = false;
	bool texWriteBound = false;
};

