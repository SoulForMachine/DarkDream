
#include "BaseLib/Math/math3d.h"
#include "Light.h"

using namespace math3d;


namespace Engine
{

	math3d::mat4f Light::_shadowMapScaleBiasMat(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	);

	Light::Light()
	{
		_position.set_null();
		_direction.set(0.0f, 0.0f, -1.0f);
		_type = LIGHT_TYPE_DIRECTIONAL;
		_color.set(1.0f, 1.0f, 1.0f);
		_cutoffAngle = math3d::PI / 4.0f;
		_attenuation = 1.0f;
		_intensity = 1.0f;
		_viewProjDirty = true;
	}

	const mat4f& Light::GetViewProjectionTransform() const
	{
		if(_viewProjDirty)
		{
			if(_type == LIGHT_TYPE_DIRECTIONAL)
			{
				mat4f view, proj;
				view.look_at(-_direction * 400.0f, vec3f::null, vec3f::y_axis);
				proj.ortho(-15.0f, 15.0f, -15.0f, 15.0f, -100.0f, 1000.0f);
				mul(_viewProjMat, view, proj);
			}
			else if(_type == LIGHT_TYPE_SPOTLIGHT)
			{
			}
			else if(_type == LIGHT_TYPE_OMNI)
			{
			}

			_viewProjDirty = false;
		}

		return _viewProjMat;
	}

	const mat4f& Light::GetViewProjectionSBTransform() const
	{
		const mat4f& view_proj = GetViewProjectionTransform();
		mul(_viewProjSBMat, view_proj, _shadowMapScaleBiasMat);
		return _viewProjSBMat;
	}

	void Light::UpdateShadowMapScaleBiasMat(int smap_width, int smap_height)
	{
		_shadowMapScaleBiasMat.transl_vec.x = 0.5f + (0.5f / (float)smap_width);
		_shadowMapScaleBiasMat.transl_vec.y = 0.5f + (0.5f / (float)smap_height);
	}

}
