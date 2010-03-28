
#ifndef _ENGINE_AI_ENTITY_H_
#define _ENGINE_AI_ENTITY_H_

#include "Engine/Common.h"
#include "Engine/ModelEntity.h"

namespace FileUtil
{
	class File;
}

class Parser;



namespace Engine
{

	class ENGINE_API AIEntity: public ModelEntity
	{
	public:
		enum Type
		{
			TYPE_MONSTER,
			TYPE_BOSS,
			TYPE_NPC,
		};

		AIEntity* CreateCopy() const;
		void Unload();

		AIScriptResPtr GetAIScriptRes() const
			{ return _aiScript; }
		bool SetAIScript(const tchar* file_name);

		ModelEntityType GetModelEntityType() const
			{ return ME_TYPE_AI; }

	protected:
		void ReadProperties(Parser& parser);
		void WriteProperties(FileUtil::File& file, const char* indent);

	private:
		Type _type;
		int _health;
		int _armor;
		AIScriptResPtr _aiScript;
	};

}


#endif // _ENGINE_AI_ENTITY_H_
