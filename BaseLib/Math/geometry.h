
#ifndef _MATH3D_GEOMETRY_H_
#define _MATH3D_GEOMETRY_H_

#include "vec4.h"


namespace math3d
{

	template<class _ST>
	void plane_from_triangle(vec4<_ST>& plane, const vec3<_ST> triangle[3])
	{
		cross(plane.rvec3, triangle[1] - triangle[0], triangle[2] - triangle[0]);
		plane.rvec3.normalize();
		plane.w = - dot(plane.rvec3, triangle[0]);
	}

	template<class _ST>
	void triangle_normal(vec3<_ST>& normal, const vec3<_ST> triangle[3])
	{
		cross(normal, triangle[1] - triangle[0], triangle[2] - triangle[0]);
		normal.normalize();
	}

	template<class _ST>
	bool point_in_triangle(const vec3<_ST>& point, const vec3<_ST> triangle[3])
	{
		vec3<_ST> u = triangle[1] - triangle[0];
		vec3<_ST> v = triangle[2] - triangle[0];
		vec3<_ST> w = point - triangle[0];

		float uv = dot(u, v);
		float wv = dot(w, v);
		float vv = dot(v, v);
		float wu = dot(w, u);
		float uu = dot(u, u);

		float denom = uv * uv - uu * vv;
		float s = (uv * wv - vv * wu) / denom;
		float t = (uv * wu - uu * wv) / denom;
		
		return (s >= _ST(0) && t >= _ST(0) && s + t <= _ST(1));
	}

	template<class _ST>
	bool point_in_triangle_2d(const vec2<_ST>& point, const vec2<_ST> triangle[3])
	{
		vec2<_ST> u = triangle[1] - triangle[0];
		vec2<_ST> v = triangle[2] - triangle[0];
		vec2<_ST> w = point - triangle[0];

		float uv = dot(u, v);
		float wv = dot(w, v);
		float vv = dot(v, v);
		float wu = dot(w, u);
		float uu = dot(u, u);

		float denom = uv * uv - uu * vv;
		float s = (uv * wv - vv * wu) / denom;
		float t = (uv * wu - uu * wv) / denom;
		
		return (s >= _ST(0) && t >= _ST(0) && s + t <= _ST(1));
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
	bool intersect_line_triangle(vec3<_ST>& result, const vec3<_ST>& line_pt, const vec3<_ST>& line_dir, const vec3<_ST> triangle[3])
	{
		// find intersection of line and plane containing the triangle
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
		if(!intersect_line_plane(pt, line_pt, line_dir, plane))
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
		[a, b] is line normal, c is signed distance from origin
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

	/*
		Evaluates the point relative to a plane (returns signed distance).
		If less than 0, the point is behind the plane,
		if greater than 0, the point is in front of plane,
		and if 0, the point is on the plane.
	*/
	template <class _ST>
	inline
	_ST point_to_plane_sgn_dist(const vec3<_ST>& point, const vec4<_ST>& plane)
	{
		return dot(point, plane.rvec3) + plane.w;
	}

	// returns absolute distance of a point from a plane
	template <class _ST>
	inline
	_ST point_to_plane_dist(const vec3<_ST>& point, const vec4<_ST>& plane)
	{
		return fabs(point_to_plane_sgn_dist(point, plane));
	}

	template <class _ST>
	inline
	void nearest_point_on_plane(vec3<_ST>& result, const vec3<_ST>& point, const vec4<_ST>& plane)
	{
		_ST d = point_to_plane_sgn_dist(point, plane);
		result = point - d * plane.rvec3;
	}

	template <class _ST>
	inline
	_ST point_to_line_sgn_dist_2d(const vec2<_ST>& point, const vec3<_ST>& line)
	{
		return dot(point, line.rvec2) + line.z;
	}

	template <class _ST>
	inline
	_ST point_to_line_dist_2d(const vec2<_ST>& point, const vec3<_ST>& line)
	{
		return fabs(point_to_line_sgn_dist_2d(point, line));
	}

	template <class _ST>
	inline
	void nearest_point_on_line_2d(vec2<_ST>& result, const vec2<_ST>& point, const vec3<_ST>& line)
	{
		_ST d = point_to_line_sgn_dist_2d(point, line);
		result = point - d * line.rvec2;
	}

	template <class _ST>
	inline
	bool point_in_rectangle_2d(const vec2<_ST>& point, const vec4<_ST>& rect)
	{
		return (point.x >= rect.x && point.x <= rect.z &&
				point.y >= rect.y && point.y <= rect.w);
	}

	template <class _ST>
	inline
	void rotate_90_ccw_2d(vec2<_ST>& vector)
	{
		_ST t = vector.x;
		vector.x = -vector.y;
		vector.y = t;
	}

	template <class _ST>
	inline
	void rotate_90_cw_2d(vec2<_ST>& vector)
	{
		_ST t = vector.x;
		vector.x = vector.y;
		vector.y = -t;
	}

}


#endif // _MATH3D_GEOMETRY_H_
