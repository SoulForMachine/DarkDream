
#include <cassert>
#include "BaseLib/Memory.h"
#include "GLASMProgram.h"

using namespace Memory;

namespace GL
{

#define STATE_MACHINE_HACK \
	if(_id != *_currentId) \
	{ \
		glBindProgramARB(_target, _id); \
		OPENGL_ERROR_CHECK \
		*_currentId = _id; \
	}

	extern GLState* __gl_state;


	bool ASMProgram::Create(ObjectType type, const char* source, const char*& error_string)
	{
		assert(__gl_state);

		switch(type)
		{
		case OBJ_ASM_VERTEX_PROGRAM:
			_currentId = &__gl_state->asmVertProg;
			break;
		case OBJ_ASM_GEOMETRY_PROGRAM:
			_currentId = &__gl_state->asmGeomProg;
			break;
		case OBJ_ASM_FRAGMENT_PROGRAM:
			_currentId = &__gl_state->asmFragProg;
			break;
		}

		glGenProgramsARB(1, &_id);
		OPENGL_ERROR_CHECK
		glBindProgramARB(type, _id);
		OPENGL_ERROR_CHECK
		*_currentId = _id;
		_target = type;

		glProgramStringARB(type, GL_PROGRAM_FORMAT_ASCII_ARB, strlen(source), source);
		bool result = (glGetError() != GL_NO_ERROR);
		GLint error_pos;
		glGetIntegerv(GL_PROGRAM_ERROR_POSITION_ARB, &error_pos);
		result = result && (error_pos == -1);

		const char* str = (const char*)glGetString(GL_PROGRAM_ERROR_STRING_ARB);
		size_t len = strlen(str);
		if(len)
		{
			char* buf = new(tempPool) char[len + 1];
			strcpy(buf, str);
			error_string = buf;
		}
		else
		{
			error_string = 0;
		}

		if(!result)
			Destroy();

		return result;
	}

	void ASMProgram::Destroy()
	{
		if(_id != 0)
		{
			if(_id == *_currentId)
			{
				glBindProgramARB(_target, 0);
				*_currentId = 0;
			}
			glDeleteProgramsARB(1, &_id);
			OPENGL_ERROR_CHECK
			_id = 0;
		}
	}

	void ASMProgram::EnvParameter(uint index, const float* param)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glProgramEnvParameter4fvARB(_target, index, param);
		OPENGL_ERROR_CHECK
	}

	void ASMProgram::EnvParameter(uint index, const int* param)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glProgramEnvParameterI4ivNV(_target, index, param);
		OPENGL_ERROR_CHECK
	}

	void ASMProgram::EnvParameter(uint index, const uint* param)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glProgramEnvParameterI4uivNV(_target, index, param);
		OPENGL_ERROR_CHECK
	}

	void ASMProgram::EnvParameterArray(uint index, uint count, const float* params)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glProgramEnvParameters4fvEXT(_target, index, count, params);
		OPENGL_ERROR_CHECK
	}

	void ASMProgram::EnvParameterArray(uint index, uint count, const int* params)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glProgramEnvParametersI4ivNV(_target, index, count, params);
		OPENGL_ERROR_CHECK
	}

	void ASMProgram::EnvParameterArray(uint index, uint count, const uint* params)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glProgramEnvParametersI4uivNV(_target, index, count, params);
		OPENGL_ERROR_CHECK
	}


	void ASMProgram::LocalParameter(uint index, const float* param)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glProgramLocalParameter4fvARB(_target, index, param);
		OPENGL_ERROR_CHECK
	}

	void ASMProgram::LocalParameter(uint index, const int* param)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glProgramLocalParameterI4ivNV(_target, index, param);
		OPENGL_ERROR_CHECK
	}

	void ASMProgram::LocalParameter(uint index, const uint* param)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glProgramLocalParameterI4uivNV(_target, index, param);
		OPENGL_ERROR_CHECK
	}

	void ASMProgram::LocalParameterArray(uint index, uint count, const float* params)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glProgramLocalParameters4fvEXT(_target, index, count, params);
		OPENGL_ERROR_CHECK
	}

	void ASMProgram::LocalParameterArray(uint index, uint count, const int* params)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glProgramLocalParametersI4ivNV(_target, index, count, params);
		OPENGL_ERROR_CHECK
	}

	void ASMProgram::LocalParameterArray(uint index, uint count, const uint* params)
	{
		assert(_id);
		STATE_MACHINE_HACK
		glProgramLocalParametersI4uivNV(_target, index, count, params);
		OPENGL_ERROR_CHECK
	}

}
