
#ifndef _ENGINE_STATIC_ENTITY_H_
#define _ENGINE_STATIC_ENTITY_H_

#include "Engine/Common.h"
#include "Engine/ModelEntity.h"


namespace FileUtil
{
	class File;
}

class Parser;


namespace Engine
{

	class ENGINE_API StaticEntity: public ModelEntity
	{
	public:
		StaticEntity();
		StaticEntity(const StaticEntity& rhs);
		~StaticEntity();
		StaticEntity& operator = (const StaticEntity& rhs);

		StaticEntity* CreateCopy() const;
		void Unload();
		ModelEntityType GetModelEntityType() const
			{ return ME_TYPE_STATIC; }

		bool GetClip() const
			{ return _clip; }
		void SetClip(bool clip)
			{ _clip = clip; }

	protected:
		void ReadProperties(Parser& parser);
		void WriteProperties(FileUtil::File& file, const char* indent);

	private:
		bool _clip;
	};

}

#endif //_ENGINE_STATIC_ENTITY_H_
