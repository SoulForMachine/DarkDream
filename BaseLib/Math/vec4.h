
/* ----------------------------------------
	File: vec4.h
	Purpose: 4 component vector class
	Author: Milan D.
	Date: 01.05.2005
   ---------------------------------------- */


#ifndef _MATH3D_VEC4_H_
#define _MATH3D_VEC4_H_

#include "mathutil.h"


namespace math3d
{

template <class _ST> class vec2;
template <class _ST> class vec3;




template <class _ST>
class vec4
{
public:
	typedef _ST scalar_t;
	static const int el_count = 4;

	vec4() {}
	vec4(_ST _x, _ST _y, _ST _z, _ST _w = _ST(1));

// conversion constructors
	explicit vec4(const vec2<_ST>& v2, _ST z = _ST(0), _ST w = _ST(1));
	explicit vec4(const vec3<_ST>& v3, _ST w = _ST(1));
	explicit vec4(const _ST* v);
	template <class _T>
		explicit vec4(const vec4<_T>& v)
	{
		set(_ST(v.x), _ST(v.y), _ST(v.z), _ST(v.w));
	}

// access
	operator _ST* () { return _data; }
	operator const _ST* () const { return _data; }

	_ST& operator [](int n) { return _data[n]; }
	_ST operator [](int n) const { return _data[n]; }

	void set(_ST x, _ST y, _ST z, _ST w = _ST(1));
	void set(const _ST* v);
	void set(const vec2<_ST>& v2, _ST _z = _ST(0), _ST _w = _ST(1));
	void set(const vec3<_ST>& v3, _ST _w = _ST(1));
	void get(_ST& x, _ST& y, _ST& z, _ST& w) const;
	void get(_ST* v) const;

// operations

	_ST length() const;
	_ST length_sq() const;
	void normalize();
	void negate();
	vec4 operator - ();
	void set_null();

	vec4& operator += (const vec4& v);
	vec4& operator -= (const vec4& v);
	vec4& operator *= (const vec4& v);
	vec4& operator /= (const vec4& v);

	vec4& operator += (_ST s);
	vec4& operator -= (_ST s);
	vec4& operator *= (_ST s);
	vec4& operator /= (_ST s);

// data
	union
	{
		_ST _data[el_count];
		struct{ _ST x, y, z, w; };
		struct{ _ST s, t, p, q; };
		struct{ _ST r, g, b, a; };
		struct{ vec2<_ST> rvec2; };
		struct{ vec3<_ST> rvec3; };
	};
};

typedef vec4<float> vec4f;
typedef vec4<double> vec4d;
typedef vec4<long double> vec4ld;
typedef vec4<int> vec4i;
typedef vec4<unsigned int> vec4ui;
typedef vec4<short> vec4s;
typedef vec4<unsigned short> vec4us;
typedef vec4<char> vec4b;
typedef vec4<unsigned char> vec4ub;


} // namespace math3d

// -------------------------------------------------------------------


#include "vec2.h"
#include "vec3.h"


namespace math3d
{


template <class _ST>
inline
vec4<_ST>::vec4(_ST _x, _ST _y, _ST _z, _ST _w = _ST(1))
{
	set(_x, _y, _z, _w);
}

template <class _ST>
inline
vec4<_ST>::vec4(const vec2<_ST>& v2, _ST _z = _ST(0), _ST _w = _ST(1))
{
	set(v2.x, v2.y, _z, _w);
}

template <class _ST>
inline
vec4<_ST>::vec4(const vec3<_ST>& v3, _ST _w = _ST(1))
{
	set(v3.x, v3.y, v3.z, _w);
}

template <class _ST>
inline
vec4<_ST>::vec4(const _ST* v)
{
	set(v);
}

template <class _ST>
inline
void vec4<_ST>::set(_ST _x, _ST _y, _ST _z, _ST _w = _ST(1))
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

template <class _ST>
inline
void vec4<_ST>::set(const _ST* v)
{
	x = v[0];
	y = v[1];
	z = v[2];
	w = v[3];
}

template <class _ST>
inline
void vec4<_ST>::set(const vec2<_ST>& v2, _ST _z, _ST _w)
{
	x = v2.x;
	y = v2.y;
	z = _z;
	w = _w;
}

template <class _ST>
inline
void vec4<_ST>::set(const vec3<_ST>& v3, _ST _w)
{
	x = v3.x;
	y = v3.y;
	z = v3.z;
	w = _w;
}

template <class _ST>
inline
void vec4<_ST>::get(_ST& _x, _ST& _y, _ST& _z, _ST& _w) const
{
	_x = x;
	_y = y;
	_z = z;
	_w = w;
}

template <class _ST>
inline
void vec4<_ST>::get(_ST* v) const
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}

template <class _ST>
inline
_ST vec4<_ST>::length() const
{
	return sqrt(x * x + y * y + z * z + w * w);
}

template <class _ST>
inline
_ST vec4<_ST>::length_sq() const
{
	return (x * x + y * y + z * z + w * w);
}


// warning: ignores null vector posibility
template <class _ST>
inline
void vec4<_ST>::normalize()
{
	_ST i_len = _ST(1) / length();
	x *= i_len;
	y *= i_len;
	z *= i_len;
	w *= i_len;
}


template <class _ST>
inline
void vec4<_ST>::negate()
{
	set(-x, -y, -z, -w);
}

template <class _ST>
inline
vec4<_ST> vec4<_ST>::operator - ()
{
	return vec4<_ST>(-x, -y, -z, -w);
}


template <class _ST>
inline
void vec4<_ST>::set_null()
{
	x = y = z = w = _ST(0);
}


//---------------------------------------------------------------------


template <class _ST>
inline
_ST dot(const vec4<_ST>& a, const vec4<_ST>& b)
{
	return  a.x * b.x +
			a.y * b.y +
			a.z * b.z +
			a.w * b.w;
}

// calculates cross product of a and b and stores result in r
// r = a X b
// warning: r must not reference same vector as a or b!
template <class _ST>
inline
vec4<_ST>& cross(vec4<_ST>& r, const vec4<_ST>& a, const vec4<_ST>& b)
{
	r.x = a.y * b.z - a.z * b.y;
	r.y = a.z * b.x - a.x * b.z;
	r.z = a.x * b.y - a.y * b.x;

	return r;
}

template <class _ST>
inline
vec4<_ST>& add(vec4<_ST>& r, const vec4<_ST>& a, const vec4<_ST>& b)
{
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;
	r.w = a.w + b.w;

	return r;
}

template <class _ST>
inline
vec4<_ST>& sub(vec4<_ST>& r, const vec4<_ST>& a, const vec4<_ST>& b)
{
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;
	r.w = a.w - b.w;

	return r;
}

template <class _ST>
inline
vec4<_ST>& mul(vec4<_ST>& r, const vec4<_ST>& a, const vec4<_ST>& b)
{
	r.x = a.x * b.x;
	r.y = a.y * b.y;
	r.z = a.z * b.z;
	r.w = a.w * b.w;

	return r;
}

template <class _ST>
inline
vec4<_ST>& div(vec4<_ST>& r, const vec4<_ST>& a, const vec4<_ST>& b)
{
	r.x = a.x / b.x;
	r.y = a.y / b.y;
	r.z = a.z / b.z;
	r.w = a.w / b.w;

	return r;
}


template <class _ST>
inline
vec4<_ST>& add(vec4<_ST>& r, const vec4<_ST>& a, _ST s)
{
	r.x = a.x + s;
	r.y = a.y + s;
	r.z = a.z + s;
	r.w = a.w + s;

	return r;
}

template <class _ST>
inline
vec4<_ST>& sub(vec4<_ST>& r, const vec4<_ST>& a, _ST s)
{
	r.x = a.x - s;
	r.y = a.y - s;
	r.z = a.z - s;
	r.w = a.w - s;

	return r;
}

template <class _ST>
inline
vec4<_ST>& mul(vec4<_ST>& r, const vec4<_ST>& a, _ST s)
{
	r.x = a.x * s;
	r.y = a.y * s;
	r.z = a.z * s;
	r.w = a.w * s;

	return r;
}

template <class _ST>
inline
vec4<_ST>& div(vec4<_ST>& r, const vec4<_ST>& a, _ST s)
{
	_ST rs = _ST(1) / s;

	r.x = a.x * rs;
	r.y = a.y * rs;
	r.z = a.z * rs;
	r.w = a.w * rs;

	return r;
}

// warning: ignores null vector posibility
template <class _ST>
inline
vec4<_ST>& normalize(vec4<_ST>& r, const vec4<_ST>& a)
{
	_ST rlen = _ST(1) / a.length();
	return mul(r, a, rlen);
}


template <class _ST>
inline
vec4<_ST>& negate(vec4<_ST>& r, const vec4<_ST>& a)
{
	r.set(-a.x, -a.y, -a.z, -a.w);
	return r;
}


template <class _ST>
inline
vec4<_ST> operator + (const vec4<_ST>& a, const vec4<_ST>& b)
{
	return vec4<_ST>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <class _ST>
inline
vec4<_ST> operator - (const vec4<_ST>& a, const vec4<_ST>& b)
{
	return vec4<_ST>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template <class _ST>
inline
vec4<_ST> operator * (const vec4<_ST>& a, const vec4<_ST>& b)
{
	return vec4<_ST>(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

template <class _ST>
inline
vec4<_ST> operator / (const vec4<_ST>& a, const vec4<_ST>& b)
{
	return vec4<_ST>(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}


template <class _ST>
inline
vec4<_ST> operator + (const vec4<_ST>& a, _ST s)
{
	return vec4<_ST>(a.x + s, a.y + s, a.z + s, a.w + s);
}

template <class _ST>
inline
vec4<_ST> operator - (const vec4<_ST>& a, _ST s)
{
	return vec4<_ST>(a.x - s, a.y - s, a.z - s, a.w - s);
}

template <class _ST>
inline
vec4<_ST> operator * (const vec4<_ST>& a, _ST s)
{
	return vec4<_ST>(a.x * s, a.y * s, a.z * s, a.w * s);
}

template <class _ST>
inline
vec4<_ST> operator * (_ST s, const vec4<_ST>& a)
{
	return vec4<_ST>(a.x * s, a.y * s, a.z * s, a.w * s);
}

template <class _ST>
inline
vec4<_ST> operator / (const vec4<_ST>& a, _ST s)
{
	_ST inv_s = _ST(1) / s;
	return vec4<_ST>(a.x * inv_s, a.y * inv_s, a.z * inv_s, a.w * inv_s);
}


template <class _ST>
inline
vec4<_ST>& vec4<_ST>::operator += (const vec4<_ST>& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;

	return *this;
}


template <class _ST>
inline
vec4<_ST>& vec4<_ST>::operator -= (const vec4<_ST>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;

	return *this;
}


template <class _ST>
inline
vec4<_ST>& vec4<_ST>::operator *= (const vec4<_ST>& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;

	return *this;
}


template <class _ST>
inline
vec4<_ST>& vec4<_ST>::operator /= (const vec4<_ST>& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;

	return *this;
}


template <class _ST>
inline
vec4<_ST>& vec4<_ST>::operator += (_ST s)
{
	x += s;
	y += s;
	z += s;
	w += s;

	return *this;
}


template <class _ST>
inline
vec4<_ST>& vec4<_ST>::operator -= (_ST s)
{
	x -= s;
	y -= s;
	z -= s;
	w -= s;

	return *this;
}


template <class _ST>
inline
vec4<_ST>& vec4<_ST>::operator *= (_ST s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;

	return *this;
}


template <class _ST>
inline
vec4<_ST>& vec4<_ST>::operator /= (_ST s)
{
	_ST inv_s = _ST(1) / s;
	x *= inv_s;
	y *= inv_s;
	z *= inv_s;
	w *= inv_s;

	return *this;
}


template <class _ST>
inline
bool operator == (const vec4<_ST>& v1, const vec4<_ST>& v2)
{
	return (v1.x == v2.x &&
			v1.y == v2.y &&
			v1.z == v2.z &&
			v1.w == v2.w);
}

template <class _ST>
inline
bool operator != (const vec4<_ST>& v1, const vec4<_ST>& v2)
{
	return !(v1 == v2);
}



} // namespace math3d



#endif // _MATH3D_VEC4_H_
