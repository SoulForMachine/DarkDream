
#ifndef _ENGINE_CAMERA_H_
#define _ENGINE_CAMERA_H_

#include "BaseLib/Math/math3d.h"
#include "Engine/Entity.h"
#include "Engine/Common.h"


class AABBox;


namespace Engine
{

	class ENGINE_API Camera: public Entity
	{
	public:
		enum ClipResult
		{
			CLIP_OUTSIDE,
			CLIP_INSIDE,
		};

		enum ClipPlane
		{
			CP_RIGHT,
			CP_LEFT,
			CP_BOTTOM,
			CP_TOP,
			CP_NEAR,
			CP_FAR,
		};

		Camera();

		void Shake();

		void LookAt(const math3d::vec3f& eye, const math3d::vec3f& at, const math3d::vec3f& up);
		void Perspective(float fovy, float aspect, float znear, float zfar);
		void SetViewingTransform(const math3d::mat4f& mat);
		void SetProjectionTransform(const math3d::mat4f& mat);
		const math3d::mat4f& GetCameraTransform() const
			{ return _viewTransform; }
		const math3d::mat4f& GetProjectionTransform() const
			{ return _projectionTransform; }
		const math3d::mat4f& GetViewProjectionTransform() const;
		virtual void SetPosition(const math3d::vec3f& pos);
		void MoveBy(const math3d::vec3f& transl);
		void RotateBy(float ax, float ay, float az);
		const math3d::vec4f& GetClipPlane(ClipPlane plane);

		virtual EntityType GetType() const
			{ return ENTITY_TYPE_CAMERA; }

		ClipResult IsInsideFrustum(const AABBox& bbox);

	private:
		void UpdateViewProjectionMat() const;

		math3d::mat4f _viewTransform;
		math3d::mat4f _projectionTransform;
		mutable math3d::mat4f _viewProjectionTransform;
		mutable bool _viewProjDirty;
		mutable math3d::vec4f _clipPlanes[6];
	};

}


#endif // _ENGINE_CAMERA_H_
