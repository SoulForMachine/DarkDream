
#include <cassert>
#include "GLQuery.h"


namespace GL
{

	void Query::Create(ObjectType type)
	{
		glGenQueriesARB(1, &_id);
		OPENGL_ERROR_CHECK
		_target = type;
	}

	void Query::Destroy()
	{
		if(_id)
		{
			glDeleteQueriesARB(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	void Query::BeginQuery()
	{
		glBeginQueryARB(_target, _id);
		OPENGL_ERROR_CHECK
	}

	void Query::EndQuery()
	{
		glEndQueryARB(_target);
		OPENGL_ERROR_CHECK
	}

	bool Query::ResultAvailable()
	{
		GLint available;
		glGetQueryObjectivARB(_id, GL_QUERY_RESULT_AVAILABLE_ARB, &available);
		OPENGL_ERROR_CHECK
		return (available != 0);
	}

	uint Query::GetResult()
	{
		GLuint result;
		glGetQueryObjectuivARB(_id, GL_QUERY_RESULT_ARB, &result);
		OPENGL_ERROR_CHECK
		return (uint)result;
	}

}
