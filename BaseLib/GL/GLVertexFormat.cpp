
#include "GLVertexFormat.h"
#include <BaseLib/Memory.h>


namespace GL
{

	VertexFormat::VertexFormat()
	{
		_descriptors = 0;
		_count = 0;
	}

	VertexFormat::~VertexFormat()
	{
		Memory::Delete(_descriptors);
		_count = 0;
	}

}
