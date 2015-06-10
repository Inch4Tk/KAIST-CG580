#pragma once

#include "StandardIncludes.h"
#include "Debug.h"

template <typename T>
class GLBuffer
{
public:
	GLBuffer(size_t elementCount, const T* data = 0, uint32_t updateMethod = GL_DYNAMIC_COPY) : 
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
	size_t GetElementCount() const
	{
		return elementCount;
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
	/// Unbinds the buffer from the specified target.
	/// </summary>
	/// <param name="target">The target.</param>
	void Unbind( GLenum target = GL_ARRAY_BUFFER ) const
	{
		glBindBuffer( target, 0 );
	}

private:
	size_t elementCount = 0;
	GLuint id = 0;
	GLuint updateMethod = GL_DYNAMIC_COPY;
};

