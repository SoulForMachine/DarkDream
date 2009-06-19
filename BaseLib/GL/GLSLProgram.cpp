

#include <cassert>
#include "BaseLib/Memory.h"
#include "GLSLProgram.h"

using namespace Memory;


namespace GL
{

	extern GLState* __gl_state;

	// ================= GLSLShader ====================

	bool GLSLShader::Create(ObjectType type, size_t count, const char** source, const char*& error_string)
	{
		error_string = 0;
		if(!source)
			return false;

		_id = glCreateShader(type);
		OPENGL_ERROR_CHECK
		if(!_id)
			return false;
		_target = type;

		glShaderSource(_id, count, source, 0);
		if(glGetError() != GL_NO_ERROR)
		{
			Destroy();
			return false;
		}

		glCompileShader(_id);
		bool result = (glGetError() == GL_NO_ERROR);

		GLint status;
		glGetShaderiv(_id, GL_COMPILE_STATUS, &status);
		OPENGL_ERROR_CHECK
		result = result && (status == GL_TRUE);

		GLint log_len;
		glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &log_len);
		OPENGL_ERROR_CHECK
		if(log_len > 1)
		{
			char* buf = new(tempPool) char[log_len];
			glGetShaderInfoLog(_id, log_len, 0, buf);
			OPENGL_ERROR_CHECK
			error_string = buf;
		}

		if(!result)
			Destroy();

		return result;
	}

	void GLSLShader::Destroy()
	{
		if(_id)
		{
			glDeleteShader(_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	// ================= GLSLProgram ====================

#define STATE_MACHINE_HACK \
	if(_id != __gl_state->glslProg) \
	{ \
		glUseProgram(_id); \
		OPENGL_ERROR_CHECK \
		__gl_state->glslProg = _id; \
	}


	bool GLSLProgram::Create(size_t count, GLSLShader** shaders)
	{
		if(!count || !shaders)
			return false;

		_id = glCreateProgram();
		OPENGL_ERROR_CHECK
		if(!_id)
			return false;
		_target = OBJ_GLSL_PROGRAM;

		for(size_t i = 0; i < count; ++i)
		{
			glAttachShader(_id, shaders[i]->_id);
			if(glGetError() != GL_NO_ERROR)
			{
				Destroy();
				return false;
			}
		}

		return true;
	}

	bool GLSLProgram::Link(const char*& error_string)
	{
		error_string = 0;

		glLinkProgram(_id);
		bool result = (glGetError() == GL_NO_ERROR);

		GLint status;
		glGetProgramiv(_id, GL_LINK_STATUS, &status);
		OPENGL_ERROR_CHECK
		result = result && (status == GL_TRUE);

		GLint log_len;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &log_len);
		OPENGL_ERROR_CHECK
		if(log_len > 1)
		{
			char* buf = new(tempPool) char[log_len];
			glGetProgramInfoLog(_id, log_len, 0, buf);
			OPENGL_ERROR_CHECK
			error_string = buf;
		}

		return result;
	}

	bool GLSLProgram::Validate(const char*& info_string)
	{
		info_string = 0;

		glValidateProgram(_id);
		OPENGL_ERROR_CHECK

		GLint status;
		glGetProgramiv(_id, GL_VALIDATE_STATUS, &status);
		OPENGL_ERROR_CHECK

		GLint log_len;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &log_len);
		OPENGL_ERROR_CHECK
		if(log_len > 1)
		{
			char* buf = new(tempPool) char[log_len];
			glGetProgramInfoLog(_id, log_len, 0, buf);
			OPENGL_ERROR_CHECK
			info_string = buf;
		}

		return (status == GL_TRUE);
	}

	void GLSLProgram::Destroy()
	{
		if(_id)
		{
			if(_id == __gl_state->glslProg)
			{
				glUseProgram(0);
				OPENGL_ERROR_CHECK
				__gl_state->glslProg = 0;
			}
			glDeleteProgram(_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	int GLSLProgram::GetAttribLocation(const char* name)
	{
		GLint location = glGetAttribLocation(_id, name);
		OPENGL_ERROR_CHECK
		return location;
	}

	void GLSLProgram::BindAttribLocation(uint index, const char* name)
	{
		glBindAttribLocation(_id, index, name);
		OPENGL_ERROR_CHECK
	}

	int GLSLProgram::GetUniformLocation(const char* name)
	{
		int location = glGetUniformLocation(_id, name);
		OPENGL_ERROR_CHECK
		return location;
	}

	void GLSLProgram::Parameter(ProgramParam param, int value)
	{
		glProgramParameteriARB(_id, param, value);
		OPENGL_ERROR_CHECK
	}

	/*
		Returns the fragment color to which out variable name was bound
		when program was last linked.
	*/
	int GLSLProgram::GetFragDataLocation(const char* name)
	{
		int location = glGetFragDataLocationEXT(_id, name);
		OPENGL_ERROR_CHECK
		return location;
	}

	void GLSLProgram::BindFragDataLocation(uint color_number, const char* name)
	{
		glBindFragDataLocationEXT(_id, color_number, name);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::TransformFeedbackVaryings(size_t count, const char** varyings, TFBufferMode mode)
	{
		glTransformFeedbackVaryingsEXT(_id, count, varyings, mode);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformBlockBinding(uint uniform_block_index, uint uniform_block_binding)
	{
		glUniformBlockBinding(_id, uniform_block_index, uniform_block_binding);
		OPENGL_ERROR_CHECK
	}
	
	uint GLSLProgram::GetUniformBlockIndex(const char* name)
	{
		uint index = glGetUniformBlockIndex(_id, name);
		OPENGL_ERROR_CHECK
		return index;
	}

	void GLSLProgram::UniformF(int location, float x)
	{
		STATE_MACHINE_HACK
		glUniform1f(location, x);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformF(int location, float x, float y)
	{
		STATE_MACHINE_HACK
		glUniform2f(location, x, y);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformF(int location, float x, float y, float z)
	{
		STATE_MACHINE_HACK
		glUniform3f(location, x, y, z);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformF(int location, float x, float y, float z, float w)
	{
		STATE_MACHINE_HACK
		glUniform4f(location, x, y, z, w);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::Uniform1FArray(int location, size_t count, const float* value)
	{
		STATE_MACHINE_HACK
		glUniform1fv(location, count, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::Uniform2FArray(int location, size_t count, const float* value)
	{
		STATE_MACHINE_HACK
		glUniform2fv(location, count, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::Uniform3FArray(int location, size_t count, const float* value)
	{
		STATE_MACHINE_HACK
		glUniform3fv(location, count, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::Uniform4FArray(int location, size_t count, const float* value)
	{
		STATE_MACHINE_HACK
		glUniform4fv(location, count, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformI(int location, int x)
	{
		STATE_MACHINE_HACK
		glUniform1i(location, x);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformI(int location, int x, int y)
	{
		STATE_MACHINE_HACK
		glUniform2i(location, x, y);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformI(int location, int x, int y, int z)
	{
		STATE_MACHINE_HACK
		glUniform3i(location, x, y, z);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformI(int location, int x, int y, int z, int w)
	{
		STATE_MACHINE_HACK
		glUniform4i(location, x, y, z, w);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::Uniform1IArray(int location, size_t count, const int* value)
	{
		STATE_MACHINE_HACK
		glUniform1iv(location, count, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::Uniform2IArray(int location, size_t count, const int* value)
	{
		STATE_MACHINE_HACK
		glUniform2iv(location, count, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::Uniform3IArray(int location, size_t count, const int* value)
	{
		STATE_MACHINE_HACK
		glUniform3iv(location, count, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::Uniform4IArray(int location, size_t count, const int* value)
	{
		STATE_MACHINE_HACK
		glUniform4iv(location, count, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformUI(int location, uint x)
	{
		STATE_MACHINE_HACK
		glUniform1uiEXT(location, x);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformUI(int location, uint x, uint y)
	{
		STATE_MACHINE_HACK
		glUniform2uiEXT(location, x, y);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformUI(int location, uint x, uint y, uint z)
	{
		STATE_MACHINE_HACK
		glUniform3uiEXT(location, x, y, z);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformUI(int location, uint x, uint y, uint z, uint w)
	{
		STATE_MACHINE_HACK
		glUniform4uiEXT(location, x, y, z, w);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::Uniform1UIArray(int location, size_t count, const uint* value)
	{
		STATE_MACHINE_HACK
		glUniform1uivEXT(location, count, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::Uniform2UIArray(int location, size_t count, const uint* value)
	{
		STATE_MACHINE_HACK
		glUniform2uivEXT(location, count, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::Uniform3UIArray(int location, size_t count, const uint* value)
	{
		STATE_MACHINE_HACK
		glUniform3uivEXT(location, count, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::Uniform4UIArray(int location, size_t count, const uint* value)
	{
		STATE_MACHINE_HACK
		glUniform4uivEXT(location, count, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformMatrix2x2(int location, size_t count, bool transpose, const float* value)
	{
		STATE_MACHINE_HACK
		glUniformMatrix2fv(location, count, transpose, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformMatrix2x3(int location, size_t count, bool transpose, const float* value)
	{
		STATE_MACHINE_HACK
		glUniformMatrix2x3fv(location, count, transpose, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformMatrix2x4(int location, size_t count, bool transpose, const float* value)
	{
		STATE_MACHINE_HACK
		glUniformMatrix2x4fv(location, count, transpose, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformMatrix3x2(int location, size_t count, bool transpose, const float* value)
	{
		STATE_MACHINE_HACK
		glUniformMatrix3x2fv(location, count, transpose, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformMatrix3x3(int location, size_t count, bool transpose, const float* value)
	{
		STATE_MACHINE_HACK
		glUniformMatrix3fv(location, count, transpose, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformMatrix3x4(int location, size_t count, bool transpose, const float* value)
	{
		STATE_MACHINE_HACK
		glUniformMatrix3x4fv(location, count, transpose, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformMatrix4x2(int location, size_t count, bool transpose, const float* value)
	{
		STATE_MACHINE_HACK
		glUniformMatrix4x2fv(location, count, transpose, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformMatrix4x3(int location, size_t count, bool transpose, const float* value)
	{
		STATE_MACHINE_HACK
		glUniformMatrix4x3fv(location, count, transpose, value);
		OPENGL_ERROR_CHECK
	}

	void GLSLProgram::UniformMatrix4x4(int location, size_t count, bool transpose, const float* value)
	{
		STATE_MACHINE_HACK
		glUniformMatrix4fv(location, count, transpose, value);
		OPENGL_ERROR_CHECK
	}

}
