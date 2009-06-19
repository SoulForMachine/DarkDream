
#ifndef _GLOBJECT_H_
#define _GLOBJECT_H_

#include "GLCommon.h"



namespace GL
{

	class Framebuffer;
	class Renderer;


	class BASELIB_API Object
	{
	public:
		Object()
			{ _id = 0; }
		~Object()
			{ }

		bool IsCreated() const
			{ return _id != 0; }
		ObjectType GetType() const
			{ return _target; }

	protected:
		GLuint _id;
		ObjectType _target;

		Object(const Object&);
		Object& operator = (const Object&);

		friend class Framebuffer;
		friend class Texture1D;
		friend class Texture2D;
		friend class Texture3D;
		friend class TextureCube;
		friend class Texture1DArray;
		friend class Texture2DArray;
		friend class TextureRectangle;
		friend class GLSLProgram;
	};

}

#endif // _GLOBJECT_H_
