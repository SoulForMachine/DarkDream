
#include <cassert>
#include "GLBuffer.h"


namespace GL
{

#define STATE_MACHINE_HACK \
	if(_id != *_currentId) \
	{ \
		glBindBufferARB(_target, _id); \
		OPENGL_ERROR_CHECK \
		*_currentId = _id; \
	}

	extern GLState* __gl_state;


	Buffer::Buffer()
	{
		_size = 0;
		_mapped = false;
	}

	bool Buffer::Create(ObjectType type, size_t size, const void* data, BufferUsage usage)
	{
		if(_id)
			return false;

		switch(type)
		{
		case OBJ_VERTEX_BUFFER:
			_currentId = &__gl_state->vertexBuf;
			break;
		case OBJ_INDEX_BUFFER:
			_currentId = &__gl_state->indexBuf;
			break;
		case OBJ_PIXEL_PACK_BUFFER:
			_currentId = &__gl_state->pixelPackBuf;
			break;
		case OBJ_PIXEL_UNPACK_BUFFER:
			_currentId = &__gl_state->pixelUnpackBuf;
			break;
		case OBJ_TEXTURE_BUFFER:
			_currentId = &__gl_state->textureBuf;
			break;
		case OBJ_UNIFORM_BUFFER:
			_currentId = &__gl_state->uniformBuf;
			break;
		case OBJ_TRANSFORM_FEEDBACK_BUFFER:
			_currentId = &__gl_state->transfFeedbackBuf;
			break;
		default:
			assert(false);
		}

		glGenBuffersARB(1, &_id);
		OPENGL_ERROR_CHECK

		if(!_id)
			return false;

		glBindBufferARB(type, _id);
		OPENGL_ERROR_CHECK
		*_currentId = _id;

		if(size > 0)
		{
			glBufferDataARB(type, size, data, usage);

			if(glGetError() != GL_NO_ERROR)
			{
				Destroy();
				return false;
			}
		}

		_target = type;
		_size = size;
		_usage = usage;

		return true;
	}

	void Buffer::Destroy()
	{
		if(_id)
		{
			if(_id == *_currentId)
			{
				glBindBufferARB(_target, 0);
				*_currentId = 0;
			}
			glDeleteBuffersARB(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
			_size = 0;
			_mapped = false;
		}
	}

	bool Buffer::BufferData(size_t size, const void* data, BufferUsage usage)
	{
		assert(_id);
		assert(!_mapped);
		STATE_MACHINE_HACK
		glBufferDataARB(_target, size, data, usage);
		_size = size;
		_usage = usage;
		return (glGetError() != GL_NO_ERROR);
	}

	void Buffer::BufferSubData(size_t offset, size_t size, const void* data)
	{
		assert(_id);
		assert(!_mapped);
		STATE_MACHINE_HACK
		glBufferSubDataARB(_target, offset, size, data);
		OPENGL_ERROR_CHECK
	}

	void Buffer::GetBufferSubData(size_t offset, size_t size, void* data)
	{
		assert(_id);
		assert(!_mapped);
		STATE_MACHINE_HACK
		glGetBufferSubDataARB(_target, offset, size, data);
		OPENGL_ERROR_CHECK
	}

	/*
		If discard is true, invalidates buffer data and allows driver to perform optimization.
		If you want to respecify the size and usage, call BufferData() with 0 for data before
		calling MapBuffer()
	*/
	void* Buffer::MapBuffer(BufferAccess access, bool discard)
	{
		assert(_id);
		assert(!_mapped);
		STATE_MACHINE_HACK
		if(discard)
		{
			BufferData(_size, 0, _usage);
			OPENGL_ERROR_CHECK
		}
		void* ptr = glMapBufferARB(_target, access);
		OPENGL_ERROR_CHECK
		_mapped = true;
		return ptr;
	}

	/*
		map_flags is a combination of BufferMapFlags
	*/
	void* Buffer::MapBufferRange(size_t offset, size_t length, uint map_flags)
	{
		assert(_id);
		assert(!_mapped);
		STATE_MACHINE_HACK
		void* ptr = glMapBufferRange(_target, offset, length, map_flags);
		OPENGL_ERROR_CHECK
		_mapped = true;
		return ptr;
	}

	void Buffer::FlushMappedBufferRange(size_t offset, size_t length)
	{
		assert(_id);
		assert(_mapped);
		STATE_MACHINE_HACK
		glFlushMappedBufferRange(_target, offset, length);
		OPENGL_ERROR_CHECK
	}

	bool Buffer::UnmapBuffer()
	{
		assert(_id);
		assert(_mapped);
		STATE_MACHINE_HACK
		bool result = (glUnmapBufferARB(_target) == GL_TRUE);
		OPENGL_ERROR_CHECK
		_mapped = false;
		return result;
	}

}
