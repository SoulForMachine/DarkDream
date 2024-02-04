
#ifndef _GLQUERY_H_
#define _GLQUERY_H_

#include "GLObject.h"

namespace GL
{

	class BASELIB_API Query: public Object
	{
	public:
		Query() {}
		~Query() {}

		void BeginQuery();
		void EndQuery();
		bool ResultAvailable();
		uint GetResult();

	private:
		friend class Renderer;

		Query(const Query&);
		Query& operator = (const Query&);

		void Create(ObjectType type);
		void Destroy();
	};

}


#endif // _GLQUERY_H_
