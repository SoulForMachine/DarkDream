
#include "BaseLib/AABBox.h"
#include "Camera.h"


namespace Engine
{

	Camera::Camera()
	{
		_viewTransform.set_identity();
		_projectionTransform.set_identity();
		_viewProjectionTransform.set_identity();
		_position.set_null();
		_viewProjDirty = false;
	}

	void Camera::Shake()
	{
	}

	Camera::ClipResult Camera::IsInsideFrustum(const AABBox& bbox)
	{
		return CLIP_INSIDE;
	}

	void Camera::LookAt(const math3d::vec3f& eye, const math3d::vec3f& at, const math3d::vec3f& up)
	{
		_viewTransform.look_at(eye, at, up);
		_position.x = -dot(_viewTransform.transl_vec.rvec3, _viewTransform.x_axis.rvec3);
		_position.y = -dot(_viewTransform.transl_vec.rvec3, _viewTransform.y_axis.rvec3);
		_position.z = -dot(_viewTransform.transl_vec.rvec3, _viewTransform.z_axis.rvec3);
		_viewProjDirty = true;
	}
	
	void Camera::Perspective(float fovy, float aspect, float znear, float zfar)
	{
		_projectionTransform.perspective(fovy, aspect, znear, zfar);
		_viewProjDirty = true;
	}

	void Camera::SetViewingTransform(const math3d::mat4f& mat)
	{
		_viewTransform = mat;
		_position.x = -dot(_viewTransform.transl_vec.rvec3, _viewTransform.x_axis.rvec3);
		_position.y = -dot(_viewTransform.transl_vec.rvec3, _viewTransform.y_axis.rvec3);
		_position.z = -dot(_viewTransform.transl_vec.rvec3, _viewTransform.z_axis.rvec3);
		_viewProjDirty = true;
	}

	void Camera::SetProjectionTransform(const math3d::mat4f& mat)
	{
		_projectionTransform = mat;
		_viewProjDirty = true;
	}

	const math3d::mat4f& Camera::GetViewProjectionTransform() const
	{
		if(_viewProjDirty)
		{
			math3d::mul(_viewProjectionTransform, _viewTransform, _projectionTransform);
			_viewProjDirty = false;
		}
		return _viewProjectionTransform;
	}

}
