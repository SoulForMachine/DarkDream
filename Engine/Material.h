
#ifndef _ENGINE_MATERIAL_H_
#define _ENGINE_MATERIAL_H_

#include "BaseLib/String.h"
#include "BaseLib/Math/math3d.h"
#include "Engine/Common.h"


namespace Engine
{

	class TextureRes;


	class ENGINE_API Material
	{
	public:
		Material();
		Material(const Material& mat);
		~Material();
		Material& operator = (const Material& mat);

		bool Load(const tchar* file_name);
		bool Save(const tchar* file_name) const;
		void Unload();

		const TextureRes* GetEmissionTexture() const
			{ return _emissionTexture; }
		void SetEmissionTexture(const TextureRes* val);

		const math3d::vec3f& GetDiffuseColor() const
			{ return _diffuseColor; }
		void SetDiffuseColor(const math3d::vec3f& val)
			{ _diffuseColor = val; UpdateFlags(); }

		const TextureRes* GetDiffuseTexture() const
			{ return _diffuseTexture; }
		void SetDiffuseTexture(const TextureRes* val);

		const TextureRes* GetNormalMap() const
			{ return _normalMap; }
		void SetNormalMap(const TextureRes* val);

		bool UsesTransparency() const
			{ return _useTransparency; }
		void UseTransparency(bool val)
			{ _useTransparency = val; UpdateFlags(); }
		float GetOpacity() const
			{ return _globalOpacity; }
		void SetOpacity(float val)
			{ _globalOpacity = val; UpdateFlags(); }
		const TextureRes* GetTransparencyTexture() const
			{ return _transpTexture; }
		void SetTransparencyTexture(const TextureRes* val);

		bool HasEmission() const
			{ return (_flags & FLAG_EMISSION) != 0; }
		bool HasDiffuse() const
			{ return (_flags & FLAG_DIFFUSE) != 0; }
		bool HasNormalMap() const
			{ return (_flags & FLAG_NORMAL_MAP) != 0; }
		bool HasTransparency() const
			{ return (_flags & FLAG_TRANSPARENCY) != 0; }

	private:
		enum
		{
			FLAG_EMISSION = 0x1,
			FLAG_DIFFUSE = 0x2,
			FLAG_NORMAL_MAP = 0x4,
			FLAG_TRANSPARENCY = 0x8,
		};

		void UpdateFlags();

		uint _flags;

		// emission
		const TextureRes* _emissionTexture;

		// diffuse
		math3d::vec3f _diffuseColor;
		const TextureRes* _diffuseTexture;

		// normal map
		const TextureRes* _normalMap;

		// transparency
		bool _useTransparency;
		float _globalOpacity;
		const TextureRes* _transpTexture;
	};

}


#endif // _ENGINE_MATERIAL_H_
