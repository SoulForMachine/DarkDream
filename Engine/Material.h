
#ifndef _ENGINE_MATERIAL_H_
#define _ENGINE_MATERIAL_H_

#include "BaseLib/String.h"
#include "BaseLib/Math/math3d.h"
#include "Engine/Common.h"
#include "Engine/ResourceManager.h"


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

		Texture2DResPtr GetEmissionTexture() const
			{ return _emissionTexture; }
		void SetEmissionTexture(Texture2DResPtr val);

		const math3d::vec3f& GetDiffuseColor() const
			{ return _diffuseColor; }
		void SetDiffuseColor(const math3d::vec3f& val)
			{ _diffuseColor = val; UpdateFlags(); }

		Texture2DResPtr GetDiffuseTexture() const
			{ return _diffuseTexture; }
		void SetDiffuseTexture(Texture2DResPtr val);

		Texture2DResPtr GetNormalMap() const
			{ return _normalMap; }
		void SetNormalMap(Texture2DResPtr val);

		bool UsesTransparency() const
			{ return _useTransparency; }
		void UseTransparency(bool val)
			{ _useTransparency = val; UpdateFlags(); }
		float GetOpacity() const
			{ return _globalOpacity; }
		void SetOpacity(float val)
			{ _globalOpacity = val; UpdateFlags(); }
		Texture2DResPtr GetTransparencyTexture() const
			{ return _transpTexture; }
		void SetTransparencyTexture(Texture2DResPtr val);

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
		Texture2DResPtr _emissionTexture;

		// diffuse
		math3d::vec3f _diffuseColor;
		Texture2DResPtr _diffuseTexture;

		// normal map
		Texture2DResPtr _normalMap;

		// transparency
		bool _useTransparency;
		float _globalOpacity;
		Texture2DResPtr _transpTexture;
	};

}


#endif // _ENGINE_MATERIAL_H_
