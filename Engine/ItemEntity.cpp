
#include "BaseLib/FileUtil.h"
#include "BaseLib/Parser.h"
#include "BaseLib/Memory.h"
#include "ItemEntity.h"

using namespace Memory;


namespace Engine
{

	const char* ItemEntity::_itemTypeNames[] =
	{
		"Health",
		"Ammo",
		"Key",
	};


	ItemEntity::ItemEntity()
	{
		_itemType = ITEM_TYPE_HEALTH;
		_amount = 25;
	}

	ItemEntity::ItemEntity(const ItemEntity& rhs)
	{
		*this = rhs;
	}
	
	ItemEntity::~ItemEntity()
	{
		Unload();
	}

	ItemEntity& ItemEntity::operator = (const ItemEntity& rhs)
	{
		if(&rhs != this)
		{
			ModelEntity::operator = (rhs);

			_itemType = rhs._itemType;
			_amount = rhs._amount;
		}

		return *this;
	}

	ItemEntity* ItemEntity::CreateCopy() const
	{
		return new(mapPool) ItemEntity(*this);
	}

	void ItemEntity::Unload()
	{
		ModelEntity::Unload();
	}

	void ItemEntity::ReadProperties(Parser& parser)
	{
		const int MAX_IDENT_LEN = 64;
		char buf[MAX_IDENT_LEN];
		parser.ExpectTokenString("itemType");
		parser.ReadIdentifier(buf, MAX_IDENT_LEN);
		_itemType = GetItemTypeFromName(buf);
		if(_itemType == ITEM_TYPE_COUNT)
			throw ParserException("Invalid item type");

		parser.ExpectTokenString("amount");
		parser.ReadInt(_amount);
	}

	void ItemEntity::WriteProperties(FileUtil::File& file, const char* indent)
	{
		file.Printf("%sitemType\t\t%s\n", indent, GetItemTypeName(_itemType));
		file.Printf("%samount\t\t%d\n", indent, _amount);
	}

	ItemEntity::ItemType ItemEntity::GetItemTypeFromName(const char* name)
	{
		for(int i = 0; i < ITEM_TYPE_COUNT; ++i)
		{
			if(!strcmp(name, _itemTypeNames[i]))
				return (ItemType)i;
		}

		return ITEM_TYPE_COUNT;
	}

	const char* ItemEntity::GetItemTypeName(ItemType type)
	{
		return (type == ITEM_TYPE_COUNT)? "": _itemTypeNames[type];
	}

}
