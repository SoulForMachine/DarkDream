
#include <cassert>
#include "GLTexture.h"
#include "GLFramebuffer.h"


namespace GL
{

	// =================== Renderbuffer ==================

#define STATE_MACHINE_HACK \
	if(_id != __gl_state->renderbuffer) \
	{ \
		glBindRenderbufferEXT(_target, _id); \
		OPENGL_ERROR_CHECK \
		__gl_state->renderbuffer = _id; \
	}

	extern GLState* __gl_state;


	bool Renderbuffer::Create()
	{
		if(_id)
			return false;

		glGenRenderbuffersEXT(1, &_id);
		OPENGL_ERROR_CHECK
		if(!_id)
			return false;

		glBindRenderbufferEXT(OBJ_RENDERBUFFER, _id);
		OPENGL_ERROR_CHECK
		__gl_state->renderbuffer = _id;
		_target = OBJ_RENDERBUFFER;

		return true;
	}

	void Renderbuffer::Destroy()
	{
		if(_id)
		{
			if(_id == __gl_state->renderbuffer)
			{
				glBindRenderbufferEXT(OBJ_RENDERBUFFER, 0);
				__gl_state->renderbuffer = 0;
			}
			glDeleteRenderbuffersEXT(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	void Renderbuffer::Storage(size_t samples, PixelFormat internal_format, size_t width, size_t height)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glRenderbufferStorageMultisampleEXT(OBJ_RENDERBUFFER, samples, internal_format, width, height);
		OPENGL_ERROR_CHECK
	}

	// =================== Framebuffer ==================

#undef STATE_MACHINE_HACK
#define STATE_MACHINE_HACK \
	if(_id != *_currentId) \
	{ \
		glBindFramebufferEXT(_target, _id); \
		OPENGL_ERROR_CHECK \
		*_currentId = _id; \
	}


	bool Framebuffer::Create(ObjectType type)
	{
		if(_id)
			return false;

		glGenFramebuffersEXT(1, &_id);
		OPENGL_ERROR_CHECK
		if(!_id)
			return false;

		switch(type)
		{
		case OBJ_DRAW_FRAMEBUFFER:
			_currentId = &__gl_state->drawFbuf;
			break;
		case OBJ_READ_FRAMEBUFFER:
			_currentId = &__gl_state->readFbuf;
			break;
		}

		glBindFramebufferEXT(type, _id);
		OPENGL_ERROR_CHECK
		*_currentId = _id;
		_target = type;

		return true;
	}

	void Framebuffer::Destroy()
	{
		if(_id)
		{
			if(_id == *_currentId)
			{
				glBindFramebufferEXT(_target, 0);
				*_currentId = 0;
			}
			glDeleteFramebuffersEXT(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	void Framebuffer::AttachTexture(RenderbufferType attachment, Texture* texture, int level)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glFramebufferTextureARB(_target, attachment, texture->_id, level);
		OPENGL_ERROR_CHECK
	}

	void Framebuffer::AttachTextureLayer(RenderbufferType attachment, Texture* texture, int level, int layer)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glFramebufferTextureLayerARB(_target, attachment, texture->_id, level, layer);
		OPENGL_ERROR_CHECK
	}

	void Framebuffer::AttachTextureFace(RenderbufferType attachment, Texture* texture, int level, CubeFace face)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glFramebufferTextureFaceARB(_target, attachment, texture->_id, level, face);
		OPENGL_ERROR_CHECK
	}

	void Framebuffer::AttachRenderbuffer(RenderbufferType attachment, Renderbuffer* renderbuffer)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glFramebufferRenderbufferEXT(_target, attachment, OBJ_RENDERBUFFER, renderbuffer->_id);
		OPENGL_ERROR_CHECK
	}

	FramebufferStatus Framebuffer::CheckStatus()
	{
		assert(_id);
		STATE_MACHINE_HACK

		FramebufferStatus status = (FramebufferStatus)glCheckFramebufferStatusEXT(_target);
		OPENGL_ERROR_CHECK
		return status;
	}

	void Framebuffer::ActiveDrawBuffers(size_t count, const RenderbufferType* buffers)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glDrawBuffersARB(count, (const GLenum*)buffers);
		OPENGL_ERROR_CHECK
	}

	void Framebuffer::ActiveReadBuffer(RenderbufferType buffer)
	{
		assert(_id);
		STATE_MACHINE_HACK

		glReadBuffer(buffer);
		OPENGL_ERROR_CHECK
	}

}
