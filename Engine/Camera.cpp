
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
		SetPosition(vec3f::null);
		_viewProjDirty = false;
	}

	void Camera::Shake()
	{
	}

	bool Camera::IsInsideFrustum(const AABBox& bbox) const
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

	bool Camera::IsInsideFrustum(const OBBox& bbox) const
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

	bool Camera::IsInsideFrustum(const vec3f& point) const
	{
		UpdateViewProjectionMat();

		for(int i = 0; i < 6; ++i)
		{
			if(point_to_plane_sgn_dist(point, _clipPlanes[i]) <= 0)
				return false;
		}

		return true;
	}

	void Camera::LookAt(const math3d::vec3f& eye, const math3d::vec3f& at, const math3d::vec3f& up)
	{
		_viewTransform.look_at(eye, at, up);
		vec3f pos;
		pos.x = -dot(_viewTransform.transl_vec.rvec3, _viewTransform.x_axis.rvec3);
		pos.y = -dot(_viewTransform.transl_vec.rvec3, _viewTransform.y_axis.rvec3);
		pos.z = -dot(_viewTransform.transl_vec.rvec3, _viewTransform.z_axis.rvec3);
		SetPosition(pos);
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
		vec3f pos;
		pos.x = -dot(_viewTransform.transl_vec.rvec3, _viewTransform.x_axis.rvec3);
		pos.y = -dot(_viewTransform.transl_vec.rvec3, _viewTransform.y_axis.rvec3);
		pos.z = -dot(_viewTransform.transl_vec.rvec3, _viewTransform.z_axis.rvec3);
		SetPosition(pos);
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
		Entity::SetPosition(pos);
		vec3f right(_viewTransform.x_axis.x, _viewTransform.y_axis.x, _viewTransform.z_axis.x);
		vec3f up(_viewTransform.x_axis.y, _viewTransform.y_axis.y, _viewTransform.z_axis.y);
		vec3f forward(_viewTransform.x_axis.z, _viewTransform.y_axis.z, _viewTransform.z_axis.z);
		_viewTransform.transl_vec.x = - dot(right, pos);
		_viewTransform.transl_vec.y = - dot(up, pos);
		_viewTransform.transl_vec.z = - dot(forward, pos);
		_viewProjDirty = true;
	}

	const vec4f& Camera::GetClipPlane(ClipPlane plane) const
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
