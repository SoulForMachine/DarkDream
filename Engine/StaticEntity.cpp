
#include "BaseLib/FileUtil.h"
#include "BaseLib/Memory.h"
#include "StaticEntity.h"

using namespace Memory;


namespace Engine
{
	StaticEntity* StaticEntity::CreateCopy() const
	{
		return new(mapPool) StaticEntity(*this);
	}

	void StaticEntity::Unload()
	{
		ModelEntity::Unload();
	}

	void StaticEntity::ReadProperties(Parser& parser)
	{
	}

	void StaticEntity::WriteProperties(FileUtil::File& file, const char* indent)
	{
	}

}
