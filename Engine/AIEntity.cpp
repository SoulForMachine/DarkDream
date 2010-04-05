
#include "BaseLib/FileUtil.h"
#include "BaseLib/Parser.h"
#include "BaseLib/Memory.h"
#include "EngineInternal.h"
#include "AIEntity.h"

using namespace Memory;


namespace Engine
{

	const char* AIEntity::_aiTypeNames[] =
	{
		"Monster",
		"Boss",
		"NPC",
	};


	AIEntity::AIEntity()
	{
		_aiType = AI_TYPE_MONSTER;
		_health = 100;
		_maxHealth = 100;
		_armor = 100;
		_maxArmor = 100;
		_aiScript = AIScriptResPtr::null;
	}

	AIEntity::AIEntity(const AIEntity& rhs)
	{
		_aiScript = AIScriptResPtr::null;
		*this = rhs;
	}

	AIEntity::~AIEntity()
	{
		Unload();
	}

	AIEntity& AIEntity::operator = (const AIEntity& rhs)
	{
		if(&rhs != this)
		{
			ModelEntity::operator = (rhs);

			_aiType = rhs._aiType;
			_health = rhs._health;
			_maxHealth = rhs._maxHealth;
			_armor = rhs._armor;
			_maxArmor = rhs._maxArmor;

			if(rhs._aiScript)
				_aiScript = engineAPI.aiScriptManager->CreateAIScript(rhs._aiScript.GetRes()->GetFileName());
			else
				_aiScript = AIScriptResPtr::null;
		}

		return *this;
	}

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
		const int MAX_IDENT_LEN = 64;
		char buf[MAX_IDENT_LEN];

		parser.ExpectTokenString("aiType");
		parser.ReadIdentifier(buf, MAX_IDENT_LEN);
		_aiType = GetAITypeFromName(buf);
		if(_aiType == AI_TYPE_COUNT)
			throw ParserException("Invalid AI type");

		parser.ExpectTokenString("health");
		parser.ReadInt(_health);
		parser.ExpectTokenString("maxHealth");
		parser.ReadInt(_maxHealth);
		parser.ExpectTokenString("armor");
		parser.ReadInt(_armor);
		parser.ExpectTokenString("maxArmor");
		parser.ReadInt(_maxArmor);

		parser.ExpectTokenString("aiScript");
		parser.ReadString(path, MAX_PATH);
		tchar* p = CharToWideChar(path);
		_aiScript = engineAPI.aiScriptManager->CreateAIScript(p);
		delete[] p;
	}

	void AIEntity::WriteProperties(FileUtil::File& file, const char* indent)
	{
		const tchar* fn;

		file.Printf("%saiType\t\t%s\n", indent, GetAITypeName(_aiType));
		file.Printf("%shealth\t\t%d\n", indent, _health);
		file.Printf("%smaxHealth\t\t%d\n", indent, _maxHealth);
		file.Printf("%sarmor\t\t%d\n", indent, _armor);
		file.Printf("%smaxArmor\t\t%d\n", indent, _maxArmor);

		fn = _aiScript? _aiScript.GetRes()->GetFileName(): _T("");
		file.Printf("%saiScript\t\t\"%ls\"\n", indent, fn);
	}

	AIEntity::AIType AIEntity::GetAITypeFromName(const char* name)
	{
		for(int i = 0; i < AI_TYPE_COUNT; ++i)
		{
			if(!strcmp(name, _aiTypeNames[i]))
				return (AIType)i;
		}

		return AI_TYPE_COUNT;
	}

	const char* AIEntity::GetAITypeName(AIType type)
	{
		return (type == AI_TYPE_COUNT)? "": _aiTypeNames[type];
	}

}
