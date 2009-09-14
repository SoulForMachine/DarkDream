
#include "BaseLib/AABBox.h"
#include "Camera.h"

using namespace math3d;


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

	void Camera::SetPosition(const math3d::vec3f& pos)
	{
		_position = pos;
		vec3f right(_viewTransform.x_axis.x, _viewTransform.y_axis.x, _viewTransform.z_axis.x);
		vec3f up(_viewTransform.x_axis.y, _viewTransform.y_axis.y, _viewTransform.z_axis.y);
		vec3f forward(_viewTransform.x_axis.z, _viewTransform.y_axis.z, _viewTransform.z_axis.z);
		_viewTransform.transl_vec.x = - dot(right, pos);
		_viewTransform.transl_vec.y = - dot(up, pos);
		_viewTransform.transl_vec.z = - dot(forward, pos);
		_viewProjDirty = true;
	}

	void Camera::MoveBy(const math3d::vec3f& transl)
	{
		vec3f right(_viewTransform.x_axis.x, _viewTransform.y_axis.x, _viewTransform.z_axis.x);
		vec3f up(_viewTransform.x_axis.y, _viewTransform.y_axis.y, _viewTransform.z_axis.y);
		vec3f forward(_viewTransform.x_axis.z, _viewTransform.y_axis.z, _viewTransform.z_axis.z);
		_position += right * transl.x + up * transl.y + forward * transl.z;
		_viewTransform.transl_vec.x = - dot(right, _position);
		_viewTransform.transl_vec.y = - dot(up, _position);
		_viewTransform.transl_vec.z = - dot(forward, _position);
		_viewProjDirty = true;
	}

	void Camera::RotateBy(float ax, float ay, float az)
	{
		mat3f rot, prev_rot(_viewTransform), result;
		rot.set_rotation_z(az);
		rot.rotate_y(ay);
		rot.rotate_x(ax);
		mul(result, prev_rot, rot);
		_viewTransform.set3x3(result);
		_viewProjDirty = true;
	}

}
