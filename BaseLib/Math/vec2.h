
/* ----------------------------------------
	File: vec2.h
	Purpose: 2 component vector class
	Author: Milan D.
	Date: 01.05.2005
   ---------------------------------------- */


#ifndef _MATH3D_VEC2_H_
#define _MATH3D_VEC2_H_

#include "mathutil.h"


namespace math3d
{

template <class _ST> class vec3;
template <class _ST> class vec4;




template <class _ST>
class vec2
{
public:
	typedef _ST scalar_t;
	static const int el_count = 2;

	vec2() {}
	vec2(_ST _x, _ST _y);

// conversion constructors
	explicit vec2(const vec3<_ST>& v3);
	explicit vec2(const vec4<_ST>& v4);
	explicit vec2(const _ST* v);
	template <class _T>
		explicit vec2(const vec2<_T>& v)
	{
		set(_ST(v.x), _ST(v.y));
	}

// access
	operator _ST* () { return _data; }
	operator const _ST* () const { return _data; }

	_ST& operator [](int n) { return _data[n]; }
	_ST operator [](int n) const { return _data[n]; }

	void set(_ST x, _ST y);
	void set(const _ST* v);
	void get(_ST& x, _ST& y) const;
	void get(_ST* v) const;

// operations

	_ST length() const;
	_ST length_sq() const;
	void normalize();
	void negate();
	vec2 operator - ();
	void set_null();

	vec2& operator += (const vec2& v);
	vec2& operator -= (const vec2& v);
	vec2& operator *= (const vec2& v);
	vec2& operator /= (const vec2& v);

	vec2& operator += (_ST s);
	vec2& operator -= (_ST s);
	vec2& operator *= (_ST s);
	vec2& operator /= (_ST s);

// data

	union
	{
		_ST _data[el_count];
		struct{ _ST x, y; };
		struct{ _ST s, t; };
	};

	static const vec2 x_axis;
	static const vec2 y_axis;
	static const vec2 null;
};

typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<long double> vec2ld;
typedef vec2<int> vec2i;
typedef vec2<unsigned int> vec2ui;
typedef vec2<short> vec2s;
typedef vec2<unsigned short> vec2us;
typedef vec2<char> vec2b;
typedef vec2<unsigned char> vec2ub;

} // namespace math3d

// -------------------------------------------------------------------


#include "vec3.h"
#include "vec4.h"


namespace math3d
{

template <class _ST>
const vec2<_ST> vec2<_ST>::x_axis(_ST(1), _ST(0));
template <class _ST>
const vec2<_ST> vec2<_ST>::y_axis(_ST(0), _ST(1));
template <class _ST>
const vec2<_ST> vec2<_ST>::null(_ST(0), _ST(0));


template <class _ST>
inline
vec2<_ST>::vec2(_ST _x, _ST _y)
{
	set(_x, _y);
}

template <class _ST>
inline
vec2<_ST>::vec2(const vec3<_ST>& v3)
{
	set(v3.x, v3.y);
}

template <class _ST>
inline
vec2<_ST>::vec2(const vec4<_ST>& v4)
{
	set(v4.x, v4.y);
}

template <class _ST>
inline
vec2<_ST>::vec2(const _ST* v)
{
	set(v);
}

template <class _ST>
inline
void vec2<_ST>::set(_ST _x, _ST _y)
{
	x = _x;
	y = _y;
}

template <class _ST>
inline
void vec2<_ST>::set(const _ST* v)
{
	x = v[0];
	y = v[1];
}

template <class _ST>
inline
void vec2<_ST>::get(_ST& _x, _ST& _y) const
{
	_x = x;
	_y = y;
}

template <class _ST>
inline
void vec2<_ST>::get(_ST* v) const
{
	v[0] = x;
	v[1] = y;
}

template <class _ST>
inline
_ST vec2<_ST>::length() const
{
	return sqrt(x * x + y * y);
}

template <class _ST>
inline
_ST vec2<_ST>::length_sq() const
{
	return (x * x + y * y);
}


// warning: ignores null vector posibility
template <class _ST>
inline
void vec2<_ST>::normalize()
{
	_ST i_len = _ST(1) / length();
	x *= i_len;
	y *= i_len;
}


template <class _ST>
inline
void vec2<_ST>::negate()
{
	set(-x, -y);
}

template <class _ST>
inline
vec2<_ST> vec2<_ST>::operator - ()
{
	return vec2<_ST>(-x, -y);
}

template <class _ST>
inline
void vec2<_ST>::set_null()
{
	x = y = _ST(0);
}


//---------------------------------------------------------------------
// operations

template <class _ST>
inline
_ST dot(const vec2<_ST>& a, const vec2<_ST>& b)
{
	return  a.x * b.x +
			a.y * b.y;
}

template <class _ST>
inline
vec2<_ST>& add(vec2<_ST>& r, const vec2<_ST>& a, const vec2<_ST>& b)
{
	r.x = a.x + b.x;
	r.y = a.y + b.y;

	return r;
}

template <class _ST>
inline
vec2<_ST>& sub(vec2<_ST>& r, const vec2<_ST>& a, const vec2<_ST>& b)
{
	r.x = a.x - b.x;
	r.y = a.y - b.y;

	return r;
}

template <class _ST>
inline
vec2<_ST>& mul(vec2<_ST>& r, const vec2<_ST>& a, const vec2<_ST>& b)
{
	r.x = a.x * b.x;
	r.y = a.y * b.y;

	return r;
}

template <class _ST>
inline
vec2<_ST>& div(vec2<_ST>& r, const vec2<_ST>& a, const vec2<_ST>& b)
{
	r.x = a.x / b.x;
	r.y = a.y / b.y;

	return r;
}


template <class _ST>
inline
vec2<_ST>& add(vec2<_ST>& r, const vec2<_ST>& a, _ST s)
{
	r.x = a.x + s;
	r.y = a.y + s;

	return r;
}

template <class _ST>
inline
vec2<_ST>& sub(vec2<_ST>& r, const vec2<_ST>& a, _ST s)
{
	r.x = a.x - s;
	r.y = a.y - s;

	return r;
}

template <class _ST>
inline
vec2<_ST>& mul(vec2<_ST>& r, const vec2<_ST>& a, _ST s)
{
	r.x = a.x * s;
	r.y = a.y * s;

	return r;
}

template <class _ST>
inline
vec2<_ST>& div(vec2<_ST>& r, const vec2<_ST>& a, _ST s)
{
	_ST rs = _ST(1) / s;

	r.x = a.x * rs;
	r.y = a.y * rs;

	return r;
}

// WARNING: ignores null vector posibility
template <class _ST>
inline
vec2<_ST>& normalize(vec2<_ST>& r, const vec2<_ST>& a)
{
	_ST rlen = _ST(1) / a.length();
	return mul(r, a, rlen);
}


template <class _ST>
inline
vec2<_ST>& negate(vec2<_ST>& r, const vec2<_ST>& a)
{
	r.set(-a.x, -a.y);
	return r;
}


template <class _ST>
inline
vec2<_ST> operator + (const vec2<_ST>& a, const vec2<_ST>& b)
{
	return vec2<_ST>(a.x + b.x, a.y + b.y);
}

template <class _ST>
inline
vec2<_ST> operator - (const vec2<_ST>& a, const vec2<_ST>& b)
{
	return vec2<_ST>(a.x - b.x, a.y - b.y);
}

template <class _ST>
inline
vec2<_ST> operator * (const vec2<_ST>& a, const vec2<_ST>& b)
{
	return vec2<_ST>(a.x * b.x, a.y * b.y);
}

template <class _ST>
inline
vec2<_ST> operator / (const vec2<_ST>& a, const vec2<_ST>& b)
{
	return vec2<_ST>(a.x / b.x, a.y / b.y);
}


template <class _ST>
inline
vec2<_ST> operator + (const vec2<_ST>& a, _ST s)
{
	return vec2<_ST>(a.x + s, a.y + s);
}

template <class _ST>
inline
vec2<_ST> operator - (const vec2<_ST>& a, _ST s)
{
	return vec2<_ST>(a.x - s, a.y - s);
}

template <class _ST>
inline
vec2<_ST> operator * (const vec2<_ST>& a, _ST s)
{
	return vec2<_ST>(a.x * s, a.y * s);
}

template <class _ST>
inline
vec2<_ST> operator * (_ST s, const vec2<_ST>& a)
{
	return vec2<_ST>(a.x * s, a.y * s);
}


template <class _ST>
inline
vec2<_ST> operator / (const vec2<_ST>& a, _ST s)
{
	_ST inv_s = _ST(1) / s;
	return vec2<_ST>(a.x * inv_s, a.y * inv_s);
}

template <class _ST>
inline
vec2<_ST>& vec2<_ST>::operator += (const vec2<_ST>& v)
{
	x += v.x;
	y += v.y;

	return *this;
}


template <class _ST>
inline
vec2<_ST>& vec2<_ST>::operator -= (const vec2<_ST>& v)
{
	x -= v.x;
	y -= v.y;

	return *this;
}


template <class _ST>
inline
vec2<_ST>& vec2<_ST>::operator *= (const vec2<_ST>& v)
{
	x *= v.x;
	y *= v.y;

	return *this;
}


template <class _ST>
inline
vec2<_ST>& vec2<_ST>::operator /= (const vec2<_ST>& v)
{
	x /= v.x;
	y /= v.y;

	return *this;
}


template <class _ST>
inline
vec2<_ST>& vec2<_ST>::operator += (_ST s)
{
	x += s;
	y += s;

	return *this;
}


template <class _ST>
inline
vec2<_ST>& vec2<_ST>::operator -= (_ST s)
{
	x -= s;
	y -= s;

	return *this;
}


template <class _ST>
inline
vec2<_ST>& vec2<_ST>::operator *= (_ST s)
{
	x *= s;
	y *= s;

	return *this;
}


template <class _ST>
inline
vec2<_ST>& vec2<_ST>::operator /= (_ST s)
{
	_ST inv_s = _ST(1) / s;
	x *= inv_s;
	y *= inv_s;

	return *this;
}


template <class _ST>
inline
bool operator == (const vec2<_ST>& v1, const vec2<_ST>& v2)
{
	return (v1.x == v2.x &&
			v1.y == v2.y);
}

template <class _ST>
inline
bool operator != (const vec2<_ST>& v1, const vec2<_ST>& v2)
{
	return !(v1 == v2);
}


} // namespace math3d



#endif // _MATH3D_VEC2_H_
