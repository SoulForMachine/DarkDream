
#ifndef _GLASMPROGRAM_H_
#define _GLASMPROGRAM_H_

#include "GLObject.h"


namespace GL
{

	class BASELIB_API ASMProgram: public Object
	{
	public:
		void EnvParameter(uint index, const float* param) const;
		void EnvParameter(uint index, const int* param) const;
		void EnvParameter(uint index, const uint* param) const;
		void EnvParameterArray(uint index, uint count, const float* params) const;
		void EnvParameterArray(uint index, uint count, const int* params) const;
		void EnvParameterArray(uint index, uint count, const uint* params) const;

		void LocalParameter(uint index, const float* param) const;
		void LocalParameter(uint index, const int* param) const;
		void LocalParameter(uint index, const uint* param) const;
		void LocalParameterArray(uint index, uint count, const float* params) const;
		void LocalParameterArray(uint index, uint count, const int* params) const;
		void LocalParameterArray(uint index, uint count, const uint* params) const;
	private:
		friend class Renderer;

		ASMProgram() {}
		~ASMProgram() {}
		ASMProgram(const ASMProgram&);
		ASMProgram& operator = (const ASMProgram&);

		bool Create(ObjectType type, const char* source, const char*& error_string);
		void Destroy();

		GLuint* _currentId;
	};

}


#endif // _GLASMPROGRAM_H_
