
#ifndef _ENGINE_ITEM_ENTITY_H_
#define _ENGINE_ITEM_ENTITY_H_

#include "Engine/Common.h"
#include "Engine/ModelEntity.h"


namespace FileUtil
{
	class File;
}

class Parser;


namespace Engine
{

	class ENGINE_API ItemEntity: public ModelEntity
	{
	public:
		enum ItemType
		{
			ITEM_TYPE_HEALTH,
			ITEM_TYPE_AMMO,
			ITEM_TYPE_KEY,

			ITEM_TYPE_COUNT
		};

		ItemEntity();
		ItemEntity(const ItemEntity& rhs);
		~ItemEntity();
		ItemEntity& operator = (const ItemEntity& rhs);

		ItemEntity* CreateCopy() const;
		void Unload();
		ModelEntityType GetModelEntityType() const
			{ return ME_TYPE_ITEM; }

		ItemType GetItemType() const
			{ return _itemType; }
		void SetItemType(ItemType type)
			{ _itemType = type; }

		int GetAmount() const
			{ return _amount; }
		void SetAmount(int amount)
			{ _amount = amount; }

	protected:
		void ReadProperties(Parser& parser);
		void WriteProperties(FileUtil::File& file, const char* indent);

	private:
		static ItemType GetItemTypeFromName(const char* name);
		static const char* GetItemTypeName(ItemType type);

		ItemType _itemType;
		int _amount;

		static const char* _itemTypeNames[];
	};

}

#endif //_ENGINE_ITEM_ENTITY_H_
