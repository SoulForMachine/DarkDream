
#ifndef _MATH3D_GEOMETRY_H_
#define _MATH3D_GEOMETRY_H_

#include "vec4.h"


namespace math3d
{

	template<class _ST>
	void plane_from_triangle(vec4<_ST>& plane, const vec3<_ST> triangle[3])
	{
		plane.rvec3 = cross(triangle[1] - triangle[0], triangle[2] - triangle[0]);
		plane.rvec3.normalize();
		plane.w = - dot(plane.rvec3, triangle[0]);
	}

	template<class _ST>
	bool intersect_line_plane(vec3<_ST>& result, const vec3<_ST>& line_pt, const vec3<_ST>& line_dir, const vec4<_ST>& plane)
	{
		float b = dot(plane.rvec3, line_dir);
		if(fcmp_eq(b, _ST(0)))
			return false; // line paralel to plane
		float a = - (dot(plane.rvec3, line_pt) + plane.w);
		result = line_pt + (a / b) * line_dir;
		return true;
	}

	template<class _ST>
	bool intersect_ray_plane(vec3<_ST>& result, const vec3<_ST>& ray_pt, const vec3<_ST>& ray_dir, const vec4<_ST>& plane)
	{
		float b = dot(plane.rvec3, ray_dir);
		if(fcmp_eq(b, _ST(0)))
			return false; // ray paralel to plane
		float a = - (dot(plane.rvec3, ray_pt) + plane.w);
		float t = a / b;
		if(t < _ST(0))
			return false; // plane behind ray
		result = ray_pt + t * ray_dir;
		return true;
	}

	template<class _ST>
	bool intersect_ray_triangle(vec3<_ST>& result, const vec3<_ST>& ray_pt, const vec3<_ST>& ray_dir, const vec3<_ST> triangle[3])
	{
		// find intersection of ray and plane containing the triangle
		vec4<_ST> plane;
		vec3<_ST> u = triangle[1] - triangle[0];
		vec3<_ST> v = triangle[2] - triangle[0];
		cross(plane.rvec3, u, v);
		float len_sq = plane.rvec3.length_sq();
		if(fcmp_eq(len_sq, _ST(0)))
			return false; // degenerate triangle
		plane.rvec3 *= 1.0f / sqrt(len_sq);
		plane.w = - dot(plane.rvec3, triangle[0]);
		vec3<_ST> pt;
		if(!intersect_ray_plane(pt, ray_pt, ray_dir, plane))
			return false;

		// check if the intersection point is within the triangle
		vec3<_ST> w = pt - triangle[0];

		float uv = dot(u, v);
		float wv = dot(w, v);
		float vv = dot(v, v);
		float wu = dot(w, u);
		float uu = dot(u, u);

		float denom = uv * uv - uu * vv;
		float s = (uv * wv - vv * wu) / denom;
		float t = (uv * wu - uu * wv) / denom;
		if(s >= _ST(0) && t >= _ST(0) && s + t <= _ST(1))
		{
			result = pt;
			return true;
		}
		else
		{
			return false;
		}
	}

	/*
		Line is represented in the form:
			ax + by + c = 0
		thus vec3 contains a, b and c parameters of the line
	*/
	template<class _ST>
	bool intersect_lines_2d(vec2<_ST>& result, const vec3<_ST>& line1, const vec3<_ST>& line2)
	{
		float denom = line1.y * line2.x - line2.y * line1.x;
		if(fcmp_eq(denom, _ST(0)))
			return false;
		result.x = (line1.z * line2.y - line2.z * line1.y) / denom;
		result.y = (line1.z * line2.x - line2.z * line1.x) / -denom;
		return true;
	}

}


#endif // _MATH3D_GEOMETRY_H_
