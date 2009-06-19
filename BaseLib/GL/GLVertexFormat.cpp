
#include "GLVertexFormat.h"


namespace GL
{

	VertexFormat::VertexFormat()
	{
		_descriptors = 0;
		_count = 0;
	}

	VertexFormat::~VertexFormat()
	{
		delete[] _descriptors;
		_count = 0;
	}

}
