
#ifndef _ENGINE_GPUPROGRAM_H_
#define _ENGINE_GPUPROGRAM_H_

#include "Engine/Common.h"


namespace GL
{
	class Renderer;
	class GLSLProgram;
	class GLSLShader;
}


namespace Engine
{

	enum
	{
		ATTRIB_POSITION,
		ATTRIB_NORMAL,
		ATTRIB_TANGENT,
		ATTRIB_BINORMAL,
		ATTRIB_COLOR,
		ATTRIB_TEXCOORD,
		ATTRIB_JOINT_WEIGHTS,
		ATTRIB_JOINT_INDICES,

		ATTRIB_COUNT
	};

	enum
	{
		UNIFORM_WORLD_MATRIX,
		UNIFORM_VIEW_MATRIX,
		UNIFORM_PROJECTION_MATRIX,
		UNIFORM_VIEW_PROJECTION_MATRIX,
		UNIFORM_WORLD_VIEW_PROJECTION_MATRIX,
		UNIFORM_WORLD_MATRIX_PALETTE,
		UNIFORM_JOINT_MATRIX_PALETTE,
		UNIFORM_SHADOW_MATRIX,

		UNIFORM_LIGHT_VEC,
		UNIFORM_EYE_POSITION,
		UNIFORM_VIEWPORT,
		UNIFORM_EMISSION_COLOR,
		UNIFORM_DIFFUSE_COLOR,
		UNIFORM_SPECULAR_COLOR,
		UNIFORM_SPECULAR_PARAMS,
		UNIFORM_TEX_COORD_OFFSET,
		UNIFORM_BATCH_PARAMS,
		UNIFORM_COMBINE_MODES,
		UNIFORM_JOINT_MAT_OFFSETS,

		UNIFORM_SAMPLER_DIFFUSE,
		UNIFORM_SAMPLER_SPECULAR,
		UNIFORM_SAMPLER_NORMAL,
		UNIFORM_SAMPLER_REFLECTION,
		UNIFORM_SAMPLER_SHADOW,

		UNIFORM_COUNT
	};


	class ENGINE_API GPUProgram
	{
	public:
		GPUProgram();

		bool Create(const char* prog_name, GL::Renderer* renderer, size_t count, const GL::GLSLShader** shaders);
		bool Link();
		bool Validate();
		void Destroy();

		GL::GLSLProgram* GetProgram()
			{ return _program; }
		operator GL::GLSLProgram* ()
			{ return _program; }
		int GetAttribLocation(int attrib_index)
			{ return _attribLocations[attrib_index]; }
		void BindAttribLocation(int attrib_index, int attrib_loc)
			{ _program->BindAttribLocation(attrib_loc, _shaderAttribNames[attrib_index]); }
		void BindFragDataLocation(uint color_number, const char* name)
			{ _program->BindFragDataLocation(color_number, name); }

		void UniformF(int uniform_index, float x)
			{ _program->UniformF(_uniformLocations[uniform_index], x); }
		void UniformF(int uniform_index, float x, float y)
			{ _program->UniformF(_uniformLocations[uniform_index], x, y); }
		void UniformF(int uniform_index, float x, float y, float z)
			{ _program->UniformF(_uniformLocations[uniform_index], x, y, z); }
		void UniformF(int uniform_index, float x, float y, float z, float w)
			{ _program->UniformF(_uniformLocations[uniform_index], x, y, z, w); }
		void Uniform1FArray(int uniform_index, size_t count, const float* value)
			{ _program->Uniform1FArray(_uniformLocations[uniform_index], count, value); }
		void Uniform2FArray(int uniform_index, size_t count, const float* value)
			{ _program->Uniform2FArray(_uniformLocations[uniform_index], count, value); }
		void Uniform3FArray(int uniform_index, size_t count, const float* value)
			{ _program->Uniform3FArray(_uniformLocations[uniform_index], count, value); }
		void Uniform4FArray(int uniform_index, size_t count, const float* value)
			{ _program->Uniform4FArray(_uniformLocations[uniform_index], count, value); }

		void UniformI(int uniform_index, int x)
			{ _program->UniformI(_uniformLocations[uniform_index], x); }
		void UniformI(int uniform_index, int x, int y)
			{ _program->UniformI(_uniformLocations[uniform_index], x, y); }
		void UniformI(int uniform_index, int x, int y, int z)
			{ _program->UniformI(_uniformLocations[uniform_index], x, y, z); }
		void UniformI(int uniform_index, int x, int y, int z, int w)
			{ _program->UniformI(_uniformLocations[uniform_index], x, y, z, w); }
		void Uniform1IArray(int uniform_index, size_t count, const int* value)
			{ _program->Uniform1IArray(_uniformLocations[uniform_index], count, value); }
		void Uniform2IArray(int uniform_index, size_t count, const int* value)
			{ _program->Uniform2IArray(_uniformLocations[uniform_index], count, value); }
		void Uniform3IArray(int uniform_index, size_t count, const int* value)
			{ _program->Uniform3IArray(_uniformLocations[uniform_index], count, value); }
		void Uniform4IArray(int uniform_index, size_t count, const int* value)
			{ _program->Uniform4IArray(_uniformLocations[uniform_index], count, value); }

		void UniformUI(int uniform_index, uint x)
			{ _program->UniformUI(_uniformLocations[uniform_index], x); }
		void UniformUI(int uniform_index, uint x, uint y)
			{ _program->UniformUI(_uniformLocations[uniform_index], x, y); }
		void UniformUI(int uniform_index, uint x, uint y, uint z)
			{ _program->UniformUI(_uniformLocations[uniform_index], x, y, z); }
		void UniformUI(int uniform_index, uint x, uint y, uint z, uint w)
			{ _program->UniformUI(_uniformLocations[uniform_index], x, y, z, w); }
		void Uniform1UIArray(int uniform_index, size_t count, const uint* value)
			{ _program->Uniform1UIArray(_uniformLocations[uniform_index], count, value); }
		void Uniform2UIArray(int uniform_index, size_t count, const uint* value)
			{ _program->Uniform2UIArray(_uniformLocations[uniform_index], count, value); }
		void Uniform3UIArray(int uniform_index, size_t count, const uint* value)
			{ _program->Uniform3UIArray(_uniformLocations[uniform_index], count, value); }
		void Uniform4UIArray(int uniform_index, size_t count, const uint* value)
			{ _program->Uniform4UIArray(_uniformLocations[uniform_index], count, value); }

		void UniformMatrix2x2(int uniform_index, size_t count, bool transpose, const float* value)
			{ _program->UniformMatrix2x2(_uniformLocations[uniform_index], count, transpose, value); }
		void UniformMatrix2x3(int uniform_index, size_t count, bool transpose, const float* value)
			{ _program->UniformMatrix2x3(_uniformLocations[uniform_index], count, transpose, value); }
		void UniformMatrix2x4(int uniform_index, size_t count, bool transpose, const float* value)
			{ _program->UniformMatrix2x4(_uniformLocations[uniform_index], count, transpose, value); }
		void UniformMatrix3x2(int uniform_index, size_t count, bool transpose, const float* value)
			{ _program->UniformMatrix3x2(_uniformLocations[uniform_index], count, transpose, value); }
		void UniformMatrix3x3(int uniform_index, size_t count, bool transpose, const float* value)
			{ _program->UniformMatrix3x3(_uniformLocations[uniform_index], count, transpose, value); }
		void UniformMatrix3x4(int uniform_index, size_t count, bool transpose, const float* value)
			{ _program->UniformMatrix3x4(_uniformLocations[uniform_index], count, transpose, value); }
		void UniformMatrix4x2(int uniform_index, size_t count, bool transpose, const float* value)
			{ _program->UniformMatrix4x2(_uniformLocations[uniform_index], count, transpose, value); }
		void UniformMatrix4x3(int uniform_index, size_t count, bool transpose, const float* value)
			{ _program->UniformMatrix4x3(_uniformLocations[uniform_index], count, transpose, value); }
		void UniformMatrix4x4(int uniform_index, size_t count, bool transpose, const float* value)
			{ _program->UniformMatrix4x4(_uniformLocations[uniform_index], count, transpose, value); }

	private:
		GPUProgram(const GPUProgram&);

		GL::Renderer* _renderer;
		GL::GLSLProgram* _program;
		int _attribLocations[ATTRIB_COUNT];
		static const char* _shaderAttribNames[ATTRIB_COUNT];
		int _uniformLocations[UNIFORM_COUNT];
		static const char* _shaderUniformNames[UNIFORM_COUNT];
		static const int MAX_PROGRAM_NAME = 32;
		char _programName[MAX_PROGRAM_NAME];
	};

}


#endif // _ENGINE_GPUPROGRAM_H_
