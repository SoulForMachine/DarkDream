
/* ----------------------------------------
	File: vec3.h
	Purpose: 3 component vector class
	Author: Milan D.
	Date: 01.05.2005
   ---------------------------------------- */


#ifndef _MATH3D_VEC3_H_
#define _MATH3D_VEC3_H_

#include "mathutil.h"


namespace math3d
{

template <class _ST> class vec2;
template <class _ST> class vec4;



template <class _ST>
class vec3
{
public:
	typedef _ST scalar_t;
	static const int el_count = 3;

	vec3() {}
	vec3(_ST _x, _ST _y, _ST _z);

// conversion constructors
	explicit vec3(const vec2<_ST>& v2, _ST z = _ST(0));
	explicit vec3(const vec4<_ST>& v4);
	explicit vec3(const _ST* v);
	template <class _T>
		explicit vec3(const vec3<_T>& v)
	{
		set(_ST(v.x), _ST(v.y), _ST(v.z));
	}

// access
	operator _ST* () { return _data; }
	operator const _ST* () const { return _data; }

	_ST& operator [](int n) { return _data[n]; }
	_ST operator [](int n) const { return _data[n]; }

	void set(_ST x, _ST y, _ST z);
	void set(const _ST* v);
	void set(const vec2<_ST>& v2, _ST _z = _ST(0));
	void get(_ST& x, _ST& y, _ST& z) const;
	void get(_ST* v) const;

// operations

	_ST length() const;
	_ST length_sq() const;
	void normalize();
	void negate();
	vec3 operator - () const;
	void set_null();

	vec3& operator += (const vec3& v);
	vec3& operator -= (const vec3& v);
	vec3& operator *= (const vec3& v);
	vec3& operator /= (const vec3& v);

	vec3& operator += (_ST s);
	vec3& operator -= (_ST s);
	vec3& operator *= (_ST s);
	vec3& operator /= (_ST s);

// data

	union
	{
		_ST _data[el_count];
		struct{ _ST x, y, z; };
		struct{ _ST s, t, p; };
		struct{ _ST r, g, b; };
		struct{ vec2<_ST> rvec2; };
	};

	static const vec3 x_axis;
	static const vec3 y_axis;
	static const vec3 z_axis;
	static const vec3 null;
};

typedef vec3<float> vec3f;
typedef vec3<double> vec3d;
typedef vec3<long double> vec3ld;
typedef vec3<int> vec3i;
typedef vec3<unsigned int> vec3ui;
typedef vec3<short> vec3s;
typedef vec3<unsigned short> vec3us;
typedef vec3<char> vec3b;
typedef vec3<unsigned char> vec3ub;


} // namespace math3d

// -------------------------------------------------------------------


#include "vec2.h"
#include "vec4.h"


namespace math3d
{

template <class _ST>
const vec3<_ST> vec3<_ST>::x_axis(_ST(1), _ST(0), _ST(0));
template <class _ST>
const vec3<_ST> vec3<_ST>::y_axis(_ST(0), _ST(1), _ST(0));
template <class _ST>
const vec3<_ST> vec3<_ST>::z_axis(_ST(0), _ST(0), _ST(1));
template <class _ST>
const vec3<_ST> vec3<_ST>::null(_ST(0), _ST(0), _ST(0));



template <class _ST>
inline
vec3<_ST>::vec3(_ST _x, _ST _y, _ST _z)
{
	set(_x, _y, _z);
}

template <class _ST>
inline
vec3<_ST>::vec3(const vec2<_ST>& v2, _ST _z)
{
	set(v2.x, v2.y, _z);
}

template <class _ST>
inline
vec3<_ST>::vec3(const vec4<_ST>& v4)
{
	set(v4.x, v4.y, v4.z);
}

template <class _ST>
inline
vec3<_ST>::vec3(const _ST* v)
{
	set(v);
}

template <class _ST>
inline
void vec3<_ST>::set(_ST _x, _ST _y, _ST _z)
{
	x = _x;
	y = _y;
	z = _z;
}

template <class _ST>
inline
void vec3<_ST>::set(const _ST* v)
{
	x = v[0];
	y = v[1];
	z = v[2];
}

template <class _ST>
inline
void vec3<_ST>::set(const vec2<_ST>& v2, _ST _z)
{
	x = v2.x;
	y = v2.y;
	z = _z;
}

template <class _ST>
inline
void vec3<_ST>::get(_ST& _x, _ST& _y, _ST& _z) const
{
	_x = x;
	_y = y;
	_z = z;
}

template <class _ST>
inline
void vec3<_ST>::get(_ST* v) const
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
}

template <class _ST>
inline
_ST vec3<_ST>::length() const
{
	return sqrt(x * x + y * y + z * z);
}

template <class _ST>
inline
_ST vec3<_ST>::length_sq() const
{
	return (x * x + y * y + z * z);
}


// warning: ignores null vector posibility
template <class _ST>
inline
void vec3<_ST>::normalize()
{
	_ST i_len = _ST(1) / length();
	x *= i_len;
	y *= i_len;
	z *= i_len;
}


template <class _ST>
inline
void vec3<_ST>::negate()
{
	set(-x, -y, -z);
}

template <class _ST>
inline
vec3<_ST> vec3<_ST>::operator - () const
{
	return vec3<_ST>(-x, -y, -z);
}


template <class _ST>
inline
void vec3<_ST>::set_null()
{
	x = y = z = _ST(0);
}


//---------------------------------------------------------------------


template <class _ST>
inline
_ST dot(const vec3<_ST>& a, const vec3<_ST>& b)
{
	return  a.x * b.x +
			a.y * b.y +
			a.z * b.z;
}

// calculates cross product of a and b and stores result in r
// r = a X b
// WARNING: r must not reference same vector as a or b!
template <class _ST>
inline
vec3<_ST>& cross(vec3<_ST>& r, const vec3<_ST>& a, const vec3<_ST>& b)
{
	r.x = a.y * b.z - a.z * b.y;
	r.y = a.z * b.x - a.x * b.z;
	r.z = a.x * b.y - a.y * b.x;

	return r;
}

template <class _ST>
inline
vec3<_ST>& add(vec3<_ST>& r, const vec3<_ST>& a, const vec3<_ST>& b)
{
	r.x = a.x + b.x;
	r.y = a.y + b.y;
	r.z = a.z + b.z;

	return r;
}

template <class _ST>
inline
vec3<_ST>& sub(vec3<_ST>& r, const vec3<_ST>& a, const vec3<_ST>& b)
{
	r.x = a.x - b.x;
	r.y = a.y - b.y;
	r.z = a.z - b.z;

	return r;
}

template <class _ST>
inline
vec3<_ST>& mul(vec3<_ST>& r, const vec3<_ST>& a, const vec3<_ST>& b)
{
	r.x = a.x * b.x;
	r.y = a.y * b.y;
	r.z = a.z * b.z;

	return r;
}

template <class _ST>
inline
vec3<_ST>& div(vec3<_ST>& r, const vec3<_ST>& a, const vec3<_ST>& b)
{
	r.x = a.x / b.x;
	r.y = a.y / b.y;
	r.z = a.z / b.z;

	return r;
}


template <class _ST>
inline
vec3<_ST>& add(vec3<_ST>& r, const vec3<_ST>& a, _ST s)
{
	r.x = a.x + s;
	r.y = a.y + s;
	r.z = a.z + s;

	return r;
}

template <class _ST>
inline
vec3<_ST>& sub(vec3<_ST>& r, const vec3<_ST>& a, _ST s)
{
	r.x = a.x - s;
	r.y = a.y - s;
	r.z = a.z - s;

	return r;
}

template <class _ST>
inline
vec3<_ST>& mul(vec3<_ST>& r, const vec3<_ST>& a, _ST s)
{
	r.x = a.x * s;
	r.y = a.y * s;
	r.z = a.z * s;

	return r;
}

template <class _ST>
inline
vec3<_ST>& div(vec3<_ST>& r, const vec3<_ST>& a, _ST s)
{
	_ST rs = _ST(1) / s;

	r.x = a.x * rs;
	r.y = a.y * rs;
	r.z = a.z * rs;

	return r;
}

// warning: ignores null vector posibility
template <class _ST>
inline
vec3<_ST>& normalize(vec3<_ST>& r, const vec3<_ST>& a)
{
	_ST rlen = _ST(1) / a.length();
	return mul(r, a, rlen);
}


template <class _ST>
inline
vec3<_ST>& negate(vec3<_ST>& r, const vec3<_ST>& a)
{
	r.set(-a.x, -a.y, -a.z);
	return r;
}


template <class _ST>
inline
vec3<_ST> operator + (const vec3<_ST>& a, const vec3<_ST>& b)
{
	return vec3<_ST>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <class _ST>
inline
vec3<_ST> operator - (const vec3<_ST>& a, const vec3<_ST>& b)
{
	return vec3<_ST>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <class _ST>
inline
vec3<_ST> operator * (const vec3<_ST>& a, const vec3<_ST>& b)
{
	return vec3<_ST>(a.x * b.x, a.y * b.y, a.z * b.z);
}

template <class _ST>
inline
vec3<_ST> operator / (const vec3<_ST>& a, const vec3<_ST>& b)
{
	return vec3<_ST>(a.x / b.x, a.y / b.y, a.z / b.z);
}


template <class _ST>
inline
vec3<_ST> operator + (const vec3<_ST>& a, _ST s)
{
	return vec3<_ST>(a.x + s, a.y + s, a.z + s);
}

template <class _ST>
inline
vec3<_ST> operator - (const vec3<_ST>& a, _ST s)
{
	return vec3<_ST>(a.x - s, a.y - s, a.z - s);
}

template <class _ST>
inline
vec3<_ST> operator * (const vec3<_ST>& a, _ST s)
{
	return vec3<_ST>(a.x * s, a.y * s, a.z * s);
}

template <class _ST>
inline
vec3<_ST> operator * (_ST s, const vec3<_ST>& a)
{
	return vec3<_ST>(a.x * s, a.y * s, a.z * s);
}

template <class _ST>
inline
vec3<_ST> operator / (const vec3<_ST>& a, _ST s)
{
	_ST inv_s = _ST(1) / s;
	return vec3<_ST>(a.x * inv_s, a.y * inv_s, a.z * inv_s);
}

template <class _ST>
inline
vec3<_ST>& vec3<_ST>::operator += (const vec3<_ST>& v)
{
	x += v.x;
	y += v.y;
	z += v.z;

	return *this;
}


template <class _ST>
inline
vec3<_ST>& vec3<_ST>::operator -= (const vec3<_ST>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;

	return *this;
}


template <class _ST>
inline
vec3<_ST>& vec3<_ST>::operator *= (const vec3<_ST>& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;

	return *this;
}


template <class _ST>
inline
vec3<_ST>& vec3<_ST>::operator /= (const vec3<_ST>& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;

	return *this;
}


template <class _ST>
inline
vec3<_ST>& vec3<_ST>::operator += (_ST s)
{
	x += s;
	y += s;
	z += s;

	return *this;
}


template <class _ST>
inline
vec3<_ST>& vec3<_ST>::operator -= (_ST s)
{
	x -= s;
	y -= s;
	z -= s;

	return *this;
}


template <class _ST>
inline
vec3<_ST>& vec3<_ST>::operator *= (_ST s)
{
	x *= s;
	y *= s;
	z *= s;

	return *this;
}


template <class _ST>
inline
vec3<_ST>& vec3<_ST>::operator /= (_ST s)
{
	_ST inv_s = _ST(1) / s;
	x *= inv_s;
	y *= inv_s;
	z *= inv_s;

	return *this;
}

template <class _ST>
inline
bool operator == (const vec3<_ST>& v1, const vec3<_ST>& v2)
{
	return (v1.x == v2.x &&
			v1.y == v2.y &&
			v1.z == v2.z);
}

template <class _ST>
inline
bool operator != (const vec3<_ST>& v1, const vec3<_ST>& v2)
{
	return !(v1 == v2);
}



} // namespace math3d



#endif // _MATH3D_VEC3_H_
