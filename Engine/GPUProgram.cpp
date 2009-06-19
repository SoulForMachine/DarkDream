
#include "BaseLib/GL/GLRenderer.h"
#include "BaseLib/Console.h"
#include "GPUProgram.h"

using namespace GL;


namespace Engine
{

	const char* GPUProgram::_shaderAttribNames[ATTRIB_COUNT] =
	{
		"vertPos",
		"vertNormal",
		"vertTangent",
		"vertBinormal",
		"vertColor",
		"vertTexCoord",
		"vertWeights",
		"vertJointIndices",
	};

	const char* GPUProgram::_shaderUniformNames[UNIFORM_COUNT] =
	{
		"worldMat",
		"viewMat",
		"projMat",
		"viewProjMat",
		"worldViewProjMat",
		"worldMatPalette",
		"jointMatPalette",
		"shadowMat",

		"lightVec",
		"eyePos",
		"viewport",
		"emissionColor",
		"diffuseColor",
		"specularColor",
		"specularParams",
		"texCoordOffset",
		"batchParams",
		"combine",
		"jointMatOffsets",

		"diffuseSamp",
		"specularSamp",
		"normalSamp",
		"reflectionSamp",
		"shadowSamp",
	};


	// ======================== GPUProgram ================================

	GPUProgram::GPUProgram()
	{
		_renderer = 0;
		_program = 0;
	}

	bool GPUProgram::Create(const char* prog_name, Renderer* renderer, size_t count, const GLSLShader** shaders)
	{
		strncpy(_programName, prog_name, MAX_PROGRAM_NAME);
		_programName[MAX_PROGRAM_NAME - 1] = '\0';
		_renderer = renderer;
		_program = renderer->CreateProgram(count, (GLSLShader**)shaders);
		if(!_program)
		{
			Console::PrintError("Failed to create program %s", _programName);
			_renderer = 0;
			return false;
		}

		return true;
	}

	bool GPUProgram::Link()
	{
		if(!_program)
			return false;

		const char* err_str;
		bool result = _program->Link(err_str);

		if(err_str)
		{
			if(result)
				Console::Print(ESC_YELLOW "Program %s link warnings" ESC_DEFAULT ":\n%s", _programName, err_str);
			else
				Console::Print(ESC_RED "Program %s link errors" ESC_DEFAULT ":\n%s", _programName, err_str);

			delete[] err_str;
		}

		// get attribute and uniform locations
		if(result)
		{
			for(int i = 0; i < ATTRIB_COUNT; ++i)
			{
				_attribLocations[i] = _program->GetAttribLocation(_shaderAttribNames[i]);
			}

			for(int i = 0; i < UNIFORM_COUNT; ++i)
			{
				_uniformLocations[i] = _program->GetUniformLocation(_shaderUniformNames[i]);
			}
		}

		return result;
	}

	bool GPUProgram::Validate()
	{
		const char* info_str;
		bool result = _program->Validate(info_str);

		if(info_str)
		{
			if(result)
				Console::Print(ESC_YELLOW "Program %s validation succeeded with aditional info" ESC_DEFAULT ":\n%s", _programName, info_str);
			else
				Console::Print(ESC_YELLOW "Program %s validation failed" ESC_DEFAULT ":\n%s", _programName, info_str);

			delete[] info_str;
		}

		return result;
	}

	void GPUProgram::Destroy()
	{
		_renderer->DestroyProgram(_program);
		_program = 0;
		_renderer = 0;
	}

}
