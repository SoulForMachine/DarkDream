
#ifndef _GLVERTEX_FORMAT_H_
#define _GLVERTEX_FORMAT_H_

#include "GLCommon.h"


namespace GL
{

	class Renderer;

	/*
		Normalized flag is used only if integer flag is false.
		If integer is true, type must be one of integer types,
		and data is treated as pure integers. If integer is false
		and type is one of integer types, data is converted to
		float directly if normalized is false, or converted to
		normalized float ([0,1] for unsigned or [-1,1] for signed
		types) if normalized flag is true.
	*/
	struct VertexAttribDesc
	{
		int stream;
		uint attribute;
		int numComponents;
		DataType type;
		bool integer;
		bool normalized;
		size_t offset;
	};

	class BASELIB_API VertexFormat
	{
	public:

	private:
		VertexFormat();
		~VertexFormat();
		VertexAttribDesc* _descriptors;
		int _count;

		friend class Renderer;
	};

}


#endif // _GLVERTEX_FORMAT_H_
