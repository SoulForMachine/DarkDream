

#ifndef _MATH3D_TRANSFORM_H_
#define _MATH3D_TRANSFORM_H_

/*
	Functions for transforming vectors with matrices and quaternions
*/


#include "mat3.h"
#include "mat4.h"
#include "quat.h"



namespace math3d
{

// ----------- vector transform by matrix ------------

template <class _ST>
void transform(vec3<_ST>& r, const vec3<_ST>& v, const mat3<_ST>& m)
{
	r.x = m(0) * v.x + m(3) * v.y + m(6) * v.z;
	r.y = m(1) * v.x + m(4) * v.y + m(7) * v.z;
	r.z = m(2) * v.x + m(5) * v.y + m(8) * v.z;
}

template <class _ST>
void transform(vec4<_ST>& r, const vec4<_ST>& v, const mat4<_ST>& m)
{
	r.x = m(0) * v.x + m(4) * v.y + m(8) * v.z + m(12) * v.w;
	r.y = m(1) * v.x + m(5) * v.y + m(9) * v.z + m(13) * v.w;
	r.z = m(2) * v.x + m(6) * v.y + m(10) * v.z + m(14) * v.w;
	r.w = m(3) * v.x + m(7) * v.y + m(11) * v.z + m(15) * v.w;
}

template <class _ST>
void transform(vec3<_ST>& r, const vec3<_ST>& v, const mat4<_ST>& m)
{
	r.x = m(0) * v.x + m(4) * v.y + m(8) * v.z + m(12);
	r.y = m(1) * v.x + m(5) * v.y + m(9) * v.z + m(13);
	r.z = m(2) * v.x + m(6) * v.y + m(10) * v.z + m(14);
}

// transforms direction vector v with 3x3 sub-matrix of matrix m
// m must be orthogonal
template <class _ST>
void transform_dir(vec3<_ST>& r, const vec3<_ST>& v, const mat4<_ST>& m)
{
	r.x = m(0) * v.x + m(4) * v.y + m(8) * v.z;
	r.y = m(1) * v.x + m(5) * v.y + m(9) * v.z;
	r.z = m(2) * v.x + m(6) * v.y + m(10) * v.z;
}


// ----------- vector transform by quaternion ------------

template <class _ST>
void rotate(vec3<_ST>& result, const vec3<_ST>& v, const quat<_ST>& q)
{
/*	quat<_ST> temp;
	mul(temp, q, quat<_ST>(vec4(v)));
	mul(result, temp, conjugate(q));*/
}

template <class _ST>
void unproject(vec3<_ST>& result, int screen_x, int screen_y, const mat4<_ST>& inv_view_proj, _ST viewport[4])
{
	vec4<_ST> ndc(
		(_ST(2.0) * ((float)screen_x - viewport[0]) - viewport[2]) / viewport[2],
		(_ST(2.0) * ((float)screen_y - viewport[1]) - viewport[3]) / viewport[3],
		_ST(-1.0), _ST(1.0));

	vec4f temp;
	transform(temp, ndc, inv_view_proj);
	result = temp.rvec3 / temp.w;
}

} // namespace math3d


#endif // _MATH3D_TRANSFORM_H_
