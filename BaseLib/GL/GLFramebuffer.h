
#ifndef _GLFRAMEBUFFER_H_
#define _GLFRAMEBUFFER_H_

#include "GLObject.h"


namespace GL
{

	class Texture;


	class BASELIB_API Renderbuffer: public Object
	{
	public:
		Renderbuffer() {}
		~Renderbuffer() {}

		void Storage(int samples, PixelFormat internal_format, int width, int height);

	private:
		friend class Renderer;

		Renderbuffer(const Renderbuffer&);
		Renderbuffer& operator = (const Renderbuffer&);

		bool Create();
		void Destroy();
	};


	class BASELIB_API Framebuffer: public Object
	{
	public:
		Framebuffer() {}

		void AttachTexture(RenderbufferType attachment, Texture* texture, int level);
		void AttachTextureLayer(RenderbufferType attachment, Texture* texture, int level, int layer);
		void AttachTextureFace(RenderbufferType attachment, Texture* texture, int level, CubeFace face);
		void AttachRenderbuffer(RenderbufferType attachment, Renderbuffer* renderbuffer);
		FramebufferStatus CheckStatus();
		void ActiveDrawBuffers(int count, const RenderbufferType* buffers);
		void ActiveReadBuffer(RenderbufferType buffer);

	private:
		friend class Renderer;

		Framebuffer(const Framebuffer&);
		Framebuffer& operator = (const Framebuffer&);

		bool Create(ObjectType type);
		void Destroy();

		GLuint* _currentId;
	};

}


#endif // _GLFRAMEBUFFER_H_
