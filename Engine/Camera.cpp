
#include "BaseLib/Bounds.h"
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

	bool Camera::IsInsideFrustum(const AABBox& bbox)
	{
		UpdateViewProjectionMat();

		for(int i = 0; i < 6; ++i)
		{
			if(point_to_plane_sgn_dist(vec3f(bbox.minPt.x, bbox.minPt.y, bbox.minPt.z), _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.maxPt.x, bbox.minPt.y, bbox.minPt.z), _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.minPt.x, bbox.maxPt.y, bbox.minPt.z), _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.maxPt.x, bbox.maxPt.y, bbox.minPt.z), _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.minPt.x, bbox.minPt.y, bbox.maxPt.z), _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.maxPt.x, bbox.minPt.y, bbox.maxPt.z), _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.minPt.x, bbox.maxPt.y, bbox.maxPt.z), _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(vec3f(bbox.maxPt.x, bbox.maxPt.y, bbox.maxPt.z), _clipPlanes[i]) > 0) continue;

			return false;
		}

		return true;
	}

	bool Camera::IsInsideFrustum(const OBBox& bbox)
	{
		UpdateViewProjectionMat();

		for(int i = 0; i < 6; ++i)
		{
			if(point_to_plane_sgn_dist(bbox.points[0], _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[1], _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[2], _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[3], _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[4], _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[5], _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[6], _clipPlanes[i]) > 0) continue;
			if(point_to_plane_sgn_dist(bbox.points[7], _clipPlanes[i]) > 0) continue;

			return false;
		}

		return true;
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
		UpdateViewProjectionMat();
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

	const vec4f& Camera::GetClipPlane(ClipPlane plane)
	{
		UpdateViewProjectionMat();
		return _clipPlanes[plane];
	}

	float Camera::GetFOV() const
	{
		return atan(1.0f / _projectionTransform(1, 1)) * 2.0f;
	}

	float Camera::GetAspectRatio() const
	{
		return _projectionTransform(1, 1) / _projectionTransform(0, 0);
	}

	void Camera::UpdateViewProjectionMat() const
	{
		if(_viewProjDirty)
		{
			math3d::mul(_viewProjectionTransform, _viewTransform, _projectionTransform);
			_viewProjDirty = false;

			// calculate world space clip planes
			_clipPlanes[CP_RIGHT] = _viewProjectionTransform.get_col(3) - _viewProjectionTransform.get_col(0);
			_clipPlanes[CP_LEFT] = _viewProjectionTransform.get_col(3) + _viewProjectionTransform.get_col(0);
			_clipPlanes[CP_BOTTOM] = _viewProjectionTransform.get_col(3) + _viewProjectionTransform.get_col(1);
			_clipPlanes[CP_TOP] = _viewProjectionTransform.get_col(3) - _viewProjectionTransform.get_col(1);
			_clipPlanes[CP_NEAR] = _viewProjectionTransform.get_col(3) + _viewProjectionTransform.get_col(2);
			_clipPlanes[CP_FAR] = _viewProjectionTransform.get_col(3) - _viewProjectionTransform.get_col(2);

			// normalize planes
			for(int i = 0; i < 6; ++i)
			{
				float rcp_len = 1.0f / _clipPlanes[i].rvec3.length();
				_clipPlanes[i] *= rcp_len;
			}
		}
	}

}
