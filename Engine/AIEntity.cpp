
#include "BaseLib/FileUtil.h"
#include "BaseLib/Parser.h"
#include "BaseLib/Memory.h"
#include "EngineInternal.h"
#include "AIEntity.h"

using namespace Memory;


namespace Engine
{

	AIEntity* AIEntity::CreateCopy() const
	{
		return new(mapPool) AIEntity(*this);
	}

	void AIEntity::Unload()
	{
		ModelEntity::Unload();

		if(_aiScript)
		{
			engineAPI.aiScriptManager->ReleaseAIScript(_aiScript);
			_aiScript = AIScriptResPtr::null;
		}
	}

	bool AIEntity::SetAIScript(const tchar* file_name)
	{
		AIScriptResPtr ai = engineAPI.aiScriptManager->CreateAIScript(file_name, true);
		if(ai)
		{
			engineAPI.aiScriptManager->ReleaseAIScript(_aiScript);
			_aiScript = ai;
			return true;
		}
		else
			return false;
	}

	void AIEntity::ReadProperties(Parser& parser)
	{
		char path[MAX_PATH];

		parser.ExpectTokenString("aiScript");
		parser.ReadString(path, MAX_PATH);
		tchar* p = CharToWideChar(path);
		_aiScript = engineAPI.aiScriptManager->CreateAIScript(p);
		delete[] p;
	}

	void AIEntity::WriteProperties(FileUtil::File& file, const char* indent)
	{
		const tchar* fn;
		fn = _aiScript? _aiScript.GetFileRes()->GetFileName(): _T("");
		file.Printf("\taiScript\t\t\"%ls\"\n\n", fn);
	}

}
