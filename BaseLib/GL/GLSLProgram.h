
#ifndef _GLSHADER_H_
#define _GLSHADER_H_

#include "GLObject.h"

namespace GL
{

	class BASELIB_API GLSLShader: public Object
	{
	public:
		GLSLShader() {}
		~GLSLShader() {}

	private:
		friend class Renderer;

		GLSLShader(const GLSLShader&);
		GLSLShader& operator = (const GLSLShader&);

		bool Create(ObjectType type, int count, const char** source, const char*& error_string);
		void Destroy();
	};


	class BASELIB_API GLSLProgram: public Object
	{
	public:
		GLSLProgram() {}
		~GLSLProgram() {}

		bool Link(const char*& error_string);
		bool Validate(const char*& info_string);

		int GetAttribLocation(const char* name);
		void BindAttribLocation(uint index, const char* name);
		int GetUniformLocation(const char* name);
		void Parameter(ProgramParam param, int value);
		int GetFragDataLocation(const char* name);
		void BindFragDataLocation(uint color_number, const char* name);
		void TransformFeedbackVaryings(int count, const char** varyings, TFBufferMode mode);
		void UniformBlockBinding(uint uniform_block_index, uint uniform_block_binding);
		uint GetUniformBlockIndex(const char* name);

		void UniformF(int location, float x);
		void UniformF(int location, float x, float y);
		void UniformF(int location, float x, float y, float z);
		void UniformF(int location, float x, float y, float z, float w);
		void Uniform1FArray(int location, int count, const float* value);
		void Uniform2FArray(int location, int count, const float* value);
		void Uniform3FArray(int location, int count, const float* value);
		void Uniform4FArray(int location, int count, const float* value);

		void UniformI(int location, int x);
		void UniformI(int location, int x, int y);
		void UniformI(int location, int x, int y, int z);
		void UniformI(int location, int x, int y, int z, int w);
		void Uniform1IArray(int location, int count, const int* value);
		void Uniform2IArray(int location, int count, const int* value);
		void Uniform3IArray(int location, int count, const int* value);
		void Uniform4IArray(int location, int count, const int* value);

		void UniformUI(int location, uint x);
		void UniformUI(int location, uint x, uint y);
		void UniformUI(int location, uint x, uint y, uint z);
		void UniformUI(int location, uint x, uint y, uint z, uint w);
		void Uniform1UIArray(int location, int count, const uint* value);
		void Uniform2UIArray(int location, int count, const uint* value);
		void Uniform3UIArray(int location, int count, const uint* value);
		void Uniform4UIArray(int location, int count, const uint* value);

		void UniformMatrix2x2(int location, int count, bool transpose, const float* value);
		void UniformMatrix2x3(int location, int count, bool transpose, const float* value);
		void UniformMatrix2x4(int location, int count, bool transpose, const float* value);
		void UniformMatrix3x2(int location, int count, bool transpose, const float* value);
		void UniformMatrix3x3(int location, int count, bool transpose, const float* value);
		void UniformMatrix3x4(int location, int count, bool transpose, const float* value);
		void UniformMatrix4x2(int location, int count, bool transpose, const float* value);
		void UniformMatrix4x3(int location, int count, bool transpose, const float* value);
		void UniformMatrix4x4(int location, int count, bool transpose, const float* value);

	private:
		friend class Renderer;

		GLSLProgram(const GLSLProgram&);
		GLSLProgram& operator = (const GLSLProgram&);

		bool Create(size_t count, GLSLShader** shaders);
		void Destroy();
	};

}


#endif // _GLSHADER_H_
