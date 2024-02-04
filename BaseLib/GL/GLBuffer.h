
#ifndef _GLBUFFERS_H_
#define _GLBUFFERS_H_

#include "GLObject.h"


namespace GL
{

	class TextureBuffer;


	class BASELIB_API Buffer: public Object
	{
	public:
		Buffer();
		~Buffer() {}

		bool BufferData(size_t size, const void* data, BufferUsage usage);
		void BufferSubData(size_t offset, size_t size, const void* data);
		void GetBufferSubData(size_t offset, size_t size, void* data);
		void* MapBuffer(BufferAccess access, bool discard);
		void* MapBufferRange(size_t offset, size_t length, uint map_flags);
		void FlushMappedBufferRange(size_t offset, size_t length);
		bool UnmapBuffer();

	private:
		Buffer(const Buffer&);
		Buffer& operator = (const Buffer&);

		bool Create(ObjectType type, size_t size, const void* data, BufferUsage usage);
		void Destroy();

		GLuint* _currentId;
		size_t _size; // size of buffer in bytes
		BufferUsage _usage;
		bool _mapped;

		friend class TextureBuffer;
		friend class Renderer;
	};

}

#endif // _GLBUFFERS_H_
