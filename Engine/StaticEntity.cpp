
#include "BaseLib/FileUtil.h"
#include "BaseLib/Memory.h"
#include "BaseLib/Parser.h"
#include "StaticEntity.h"

using namespace Memory;


namespace Engine
{

	StaticEntity::StaticEntity()
	{
		_clip = true;
	}

	StaticEntity::StaticEntity(const StaticEntity& rhs)
	{
		*this = rhs;
	}

	StaticEntity::~StaticEntity()
	{
		Unload();
	}

	StaticEntity& StaticEntity::operator = (const StaticEntity& rhs)
	{
		if(&rhs != this)
		{
			ModelEntity::operator = (rhs);

			_clip = rhs._clip;
		}

		return *this;
	}

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
		parser.ExpectTokenString("clip");
		parser.ReadBool(_clip);
	}

	void StaticEntity::WriteProperties(FileUtil::File& file, const char* indent)
	{
		file.Printf("%sclip\t\t%s\n", indent, _clip? "True": "False");
	}

}
