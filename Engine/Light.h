
#ifndef _ENGINE_LIGHT_H_
#define _ENGINE_LIGHT_H_

#include "BaseLib/Math/math3d.h"
#include "Engine/Entity.h"
#include "Engine/Common.h"


namespace Engine
{

	class ENGINE_API Light: public Entity
	{
	public:
		enum LightType
		{
			LIGHT_TYPE_OMNI,
			LIGHT_TYPE_SPOTLIGHT,
			LIGHT_TYPE_DIRECTIONAL,
		};

		Light();

		virtual void SetPosition(const math3d::vec3f& pos)
			{ Entity::SetPosition(pos); _viewProjDirty = true; }
		void SetDirection(const math3d::vec3f& dir)
			{ _direction = dir; _viewProjDirty = true; }
		const math3d::vec3f& GetDirection() const
			{ return _direction; }
		void SetLightType(LightType type)
			{ _type = type; _viewProjDirty = true; }
		LightType GetLightType() const
			{ return _type; }
		void SetColor(const math3d::vec3f& color)
			{ _color = color; }
		const math3d::vec3f& GetColor() const
			{ return _color; }
		void SetCutoffAngle(float angle)
			{ _cutoffAngle = angle; _viewProjDirty = true; }
		float GetCutoffAngle() const
			{ return _cutoffAngle; }
		void SetAttenuation(float atten)
			{ _attenuation = atten; }
		float GetAttenuation() const
			{ return _attenuation; }
		void SetIntensity(float intensity)
			{ _intensity = intensity; }
		float GetIntensity() const
			{ return _intensity; }

		virtual EntityType GetType() const
			{ return ENTITY_TYPE_LIGHT; }

		const math3d::mat4f& GetViewProjectionTransform() const;
		const math3d::mat4f& GetViewProjectionSBTransform() const;

		static void UpdateShadowMapScaleBiasMat(int smap_width, int smap_height);

	private:
		math3d::vec3f _direction; // only valid for spotlight and directional
		LightType _type;
		math3d::vec3f _color;
		float _cutoffAngle; // spotlight cut-off angle, in radians, max PI/4
		float _attenuation;
		float _intensity;
		mutable math3d::mat4f _viewProjMat;
		mutable math3d::mat4f _viewProjSBMat;
		mutable bool _viewProjDirty;
		static math3d::mat4f _shadowMapScaleBiasMat;
	};

}


#endif // _ENGINE_LIGHT_H_
