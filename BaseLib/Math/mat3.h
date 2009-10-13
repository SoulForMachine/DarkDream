
/* ----------------------------------------
	File: mat3.h
	Purpose: 3x3 matrix class
	Author: Milan D.
	Date: 01.05.2005
   ---------------------------------------- */



#ifndef _MATH3D_MAT3_H_
#define _MATH3D_MAT3_H_

#include <cstring>


/*

Elements are ordered in memory like this:

    0    1    2

    3    4    5

    6    7    8

What matrix elements represent:

                 +------------+
    x axes ->    | Xx  Xy  Xz |
                 +------------+
    y axes ->    | Yx  Yy  Yz |
                 +------------+
    z axes ->    | Zx  Zy  Zz |
                 +------------+
*/


namespace math3d
{

template <class _ST> class vec3;
template <class _ST> class mat4;

template <class _ST>
class mat3
{
public:
	typedef _ST scalar_t;
	static const int el_count = 9;


	mat3() {}
	mat3(_ST m0, _ST m1, _ST m2,
		 _ST m3, _ST m4, _ST m5,
		 _ST m6, _ST m7, _ST m8);
	mat3(const vec3<_ST>& x, const vec3<_ST>& y, const vec3<_ST>& z);

// conversion constructors
	explicit mat3(const mat4<_ST>& m4);
	explicit mat3(const _ST* m);
	template <class _T>
		explicit mat3(const mat3<_T>& m)
	{
		set(_ST(m(0)), _ST(m(1)), _ST(m(2)), 
			_ST(m(3)), _ST(m(4)), _ST(m(5)), 
			_ST(m(6)), _ST(m(7)), _ST(m(8)));
	}

// access

	vec3<_ST>& operator [] (int i);
	const vec3<_ST>& operator [] (int i) const;
	_ST& operator () (int i);
	_ST operator () (int i) const;
	_ST& operator () (int row, int col);
	_ST operator () (int row, int col) const;
	operator _ST* () { return _data; }
	operator const _ST* () const { return _data; }

	void set(_ST m0, _ST m1, _ST m2,
			 _ST m3, _ST m4, _ST m5,
			 _ST m6, _ST m7, _ST m8);
	void set(const vec3<_ST>& x, const vec3<_ST>& y, const vec3<_ST>& z);
	void set(const mat4<_ST>& m4);
	void get(vec3<_ST>& x, vec3<_ST>& y, vec3<_ST>& z) const;
	void get(mat4<_ST>& m4) const;
	vec3<_ST> get_col(int col) const;


// operations

	_ST determinant() const;
	void transpose();
	void inverse();
	void inverse_transpose();
	void adjoint();
	void get_transpose(mat3& m) const;
	void get_inverse(mat3& m) const;
	void get_inverse_transpose(mat3& m) const;
	void get_adjoint(mat3& m) const;
	void set_identity();
	void set_transpose(const mat3& m);
	void set_inverse(const mat3& m);
	void set_inverse_transpose(const mat3& m);
	void set_adjoint(const mat3& m);

// transformations
	void rotate(_ST angle, _ST _x, _ST _y, _ST _z);
	void rotate(_ST angle, const vec3<_ST>& axis);
	void rotate_x(_ST angle);
	void rotate_y(_ST angle);
	void rotate_z(_ST angle);
	void from_euler(_ST x_angle, _ST y_angle, _ST z_angle);
	void to_euler(_ST& x_angle, _ST& y_angle, _ST& z_angle);
	void scale(_ST s);
	void scale(_ST _x, _ST _y, _ST _z);
	void scale(const vec3<_ST>& s);
	void set_rotation(_ST angle, _ST _x, _ST _y, _ST _z);
	void set_rotation(_ST angle, const vec3<_ST>& axis);
	void set_rotation_x(_ST angle);
	void set_rotation_y(_ST angle);
	void set_rotation_z(_ST angle);
	void set_scale(_ST s);
	void set_scale(_ST _x, _ST _y, _ST _z);
	void set_scale(const vec3<_ST>& s);

	union
	{
		_ST _data[el_count];
		struct
		{
			vec3<_ST> row0;
			vec3<_ST> row1;
			vec3<_ST> row2;
		};
	};

	static const mat3 identity;

private:

	void _copy(const _ST* src);
	static void _inverse(_ST* dest, const _ST* src);
	static void _transpose(_ST* dest, const _ST* src);
	static void _inverse_transpose(_ST* dest, const _ST* src);
	static void _adjoint(_ST* dest, const _ST* src);
	static void _swap(_ST& a, _ST& b);
};


typedef mat3<float> mat3f;
typedef mat3<double> mat3d;
typedef mat3<long double> mat3ld;
typedef mat3<int> mat3i;
typedef mat3<unsigned int> mat3ui;
typedef mat3<short> mat3s;
typedef mat3<unsigned short> mat3us;
typedef mat3<char> mat3b;
typedef mat3<unsigned char> mat3ub;


} // namespace math3d

// ------------------------------------

#include "vec3.h"
#include "mat4.h"


namespace math3d
{

template <class _ST>
const mat3<_ST> mat3<_ST>::identity
(
	_ST(1), _ST(0), _ST(0),
	_ST(0), _ST(1), _ST(0),
	_ST(0), _ST(0), _ST(1)
);


template <class _ST>
inline
mat3<_ST>::mat3(_ST m0, _ST m1, _ST m2,
				_ST m3, _ST m4, _ST m5,
				_ST m6, _ST m7, _ST m8)
{
	set(m0, m1, m2, m3, m4, m5, m6, m7, m8);
}

template <class _ST>
inline
mat3<_ST>::mat3(const vec3<_ST>& x, const vec3<_ST>& y, const vec3<_ST>& z)
{
	set(x, y, z);
}

// conversion constructors
template <class _ST>
inline
mat3<_ST>::mat3(const mat4<_ST>& m4)
{
	set(m4);
}

template <class _ST>
inline
mat3<_ST>::mat3(const _ST* m)
{
	_copy(m);
}


// access

template <class _ST>
inline
vec3<_ST>& mat3<_ST>::operator [] (int i)
{
	return *(vec3<_ST>*) (_data + (i << 1) + i);
}

template <class _ST>
inline
const vec3<_ST>& mat3<_ST>::operator [] (int i) const
{
	return *(vec3<_ST>*) (_data + (i << 1) + i);
}

template <class _ST>
inline
_ST& mat3<_ST>::operator () (int i)
{
	return _data[i];
}

template <class _ST>
inline
_ST mat3<_ST>::operator () (int i) const
{
	return _data[i];
}

template <class _ST>
inline
_ST& mat3<_ST>::operator () (int row, int col)
{
	return _data[(row << 1) + row + col];
}

template <class _ST>
inline
_ST mat3<_ST>::operator () (int row, int col) const
{
	return _data[(row << 1) + row + col];
}

template <class _ST>
inline
void mat3<_ST>::set(_ST m0, _ST m1, _ST m2,
					_ST m3, _ST m4, _ST m5,
					_ST m6, _ST m7, _ST m8)
{
	_data[0] = m0;
	_data[1] = m1;
	_data[2] = m2;

	_data[3] = m3;
	_data[4] = m4;
	_data[5] = m5;

	_data[6] = m6;
	_data[7] = m7;
	_data[8] = m8;
}

template <class _ST>
void mat3<_ST>::set(const vec3<_ST>& x, const vec3<_ST>& y, const vec3<_ST>& z)
{
	row0 = x;
	row1 = y;
	row2 = z;
}

template <class _ST>
void mat3<_ST>::set(const mat4<_ST>& m4)
{
	row0 = m4.row0.rvec3;
	row1 = m4.row1.rvec3;
	row2 = m4.row2.rvec3;
}

template <class _ST>
void mat3<_ST>::get(vec3<_ST>& x, vec3<_ST>& y, vec3<_ST>& z) const
{
	x = row0;
	y = row1;
	z = row2;
}

template <class _ST>
void mat3<_ST>::get(mat4<_ST>& m4) const
{
	m4.row0.rvec3 = row0;
	m4.row1.rvec3 = row1;
	m4.row2.rvec3 = row2;
}

template <class _ST>
inline
vec3<_ST> mat3<_ST>::get_col(int col) const
{
	return vec3<_ST>(_data[col], _data[col + 3], _data[col + 6]);
}


// operations

template <class _ST>
_ST mat3<_ST>::determinant() const
{
	// co-factors
	_ST cf0 = _data[4] * _data[8] - _data[5] * _data[7];
	_ST cf1 = _data[3] * _data[8] - _data[5] * _data[6];
	_ST cf2 = _data[3] * _data[7] - _data[4] * _data[6];

	return _data[0] * cf0 - _data[1] * cf1 + _data[2] * cf2;
}

template <class _ST>
inline
void mat3<_ST>::set_identity()
{
	_copy(identity);
}

template <class _ST>
void mat3<_ST>::transpose()
{
	_swap(_data[1], _data[3]);
	_swap(_data[2], _data[6]);
	_swap(_data[5], _data[7]);
}

// inverse of an arbitrary matrix
// WARNING: does not check if the matrix is invertible!
template <class _ST>
inline
void mat3<_ST>::inverse()
{
	_inverse(_data, _data);
}

template <class _ST>
inline
void mat3<_ST>::inverse_transpose()
{
	_inverse_transpose(_data, _data);
}

template <class _ST>
inline
void mat3<_ST>::adjoint()
{
	_adjoint(_data, _data);
}

template <class _ST>
inline
void mat3<_ST>::get_transpose(mat3& m) const
{
	_transpose(m._data, _data);
}

template <class _ST>
inline
void mat3<_ST>::get_inverse(mat3& m) const
{
	_inverse(m._data, _data);
}

template <class _ST>
inline
void mat3<_ST>::get_inverse_transpose(mat3& m) const
{
	_inverse_transpose(m._data, _data);
}

template <class _ST>
inline
void mat3<_ST>::get_adjoint(mat3& m) const
{
	_adjoint(m._data, _data);
}

template <class _ST>
inline
void mat3<_ST>::set_transpose(const mat3& m)
{
	_transpose(_data, m._data);
}

template <class _ST>
inline
void mat3<_ST>::set_inverse(const mat3& m)
{
	_inverse(_data, m._data);
}

template <class _ST>
inline
void mat3<_ST>::set_inverse_transpose(const mat3& m)
{
	_inverse_transpose(_data, m._data);
}

template <class _ST>
inline
void mat3<_ST>::set_adjoint(const mat3& m)
{
	_adjoint(_data, m._data);
}

// transformations

template <class _ST>
void mat3<_ST>::rotate(_ST angle, _ST _x, _ST _y, _ST _z)
{
	_ST cos_a = cos(angle);
	_ST inv_cos_a = _ST(1) - cos_a;
	_ST inv_cos_a_xy = inv_cos_a * _x * _y;
	_ST inv_cos_a_xz = inv_cos_a * _x * _z;
	_ST inv_cos_a_yz = inv_cos_a * _y * _z;
	_ST sin_a = sin(angle);
	_ST sin_a_x = sin_a * _x;
	_ST sin_a_y = sin_a * _y;
	_ST sin_a_z = sin_a * _z;

	_ST rot00 = cos_a + inv_cos_a * _x * _x;
	_ST rot10 = inv_cos_a_xy - sin_a_z;
	_ST rot20 = inv_cos_a_xz + sin_a_y;
	_ST rot01 = inv_cos_a_xy + sin_a_z;
	_ST rot11 = cos_a + inv_cos_a * _y * _y;
	_ST rot21 = inv_cos_a_yz - sin_a_x;
	_ST rot02 = inv_cos_a_xz - sin_a_y;
	_ST rot12 = inv_cos_a_yz + sin_a_x;
	_ST rot22 = cos_a + inv_cos_a * _z * _z;

	mat3<_ST> temp;

	temp(0) = _data[0] * rot00 + _data[1] * rot10 + _data[2] * rot20;
	temp(1) = _data[0] * rot01 + _data[1] * rot11 + _data[2] * rot21;
	temp(2) = _data[0] * rot02 + _data[1] * rot12 + _data[2] * rot22;

	temp(3) = _data[3] * rot00 + _data[4] * rot10 + _data[5] * rot20;
	temp(4) = _data[3] * rot01 + _data[4] * rot11 + _data[5] * rot21;
	temp(5) = _data[3] * rot02 + _data[4] * rot12 + _data[5] * rot22;

	temp(6) = _data[6] * rot00 + _data[7] * rot10 + _data[8] * rot20;
	temp(7) = _data[6] * rot01 + _data[7] * rot11 + _data[8] * rot21;
	temp(8) = _data[6] * rot02 + _data[7] * rot12 + _data[8] * rot22;

	*this = temp;
}

template <class _ST>
inline
void mat3<_ST>::rotate(_ST angle, const vec3<_ST>& axis)
{
	rotate(angle, axis.x, axis.y, axis.z);
}

template <class _ST>
void mat3<_ST>::rotate_x(_ST angle)
{
	_ST cos_a = cos(angle);
	_ST sin_a = sin(angle);
	_ST t;

	t = _data[1] * cos_a - _data[2] * sin_a;
	_data[2] = _data[1] * sin_a + _data[2] * cos_a;
	_data[1] = t;

	t = _data[4] * cos_a - _data[5] * sin_a;
	_data[5] = _data[4] * sin_a + _data[5] * cos_a;
	_data[4] = t;

	t = _data[7] * cos_a - _data[8] * sin_a;
	_data[8] = _data[7] * sin_a + _data[8] * cos_a;
	_data[7] = t;
}

template <class _ST>
void mat3<_ST>::rotate_y(_ST angle)
{
	_ST cos_a = cos(angle);
	_ST sin_a = sin(angle);
	_ST t;

	t = _data[0] * cos_a + _data[2] * sin_a;
	_data[2] = _data[2] * cos_a - _data[0] * sin_a;
	_data[0] = t;

	t = _data[3] * cos_a + _data[5] * sin_a;
	_data[5] = _data[5] * cos_a - _data[3] * sin_a;
	_data[3] = t;

	t = _data[6] * cos_a + _data[8] * sin_a;
	_data[8] = _data[8] * cos_a - _data[6] * sin_a;
	_data[6] = t;
}

template <class _ST>
void mat3<_ST>::rotate_z(_ST angle)
{
	_ST cos_a = cos(angle);
	_ST sin_a = sin(angle);
	_ST t;

	t = _data[0] * cos_a - _data[1] * sin_a;
	_data[1] = _data[0] * sin_a + _data[1] * cos_a;
	_data[0] = t;

	t = _data[3] * cos_a - _data[4] * sin_a;
	_data[4] = _data[3] * sin_a + _data[4] * cos_a;
	_data[3] = t;

	t = _data[7] * cos_a - _data[8] * sin_a;
	_data[8] = _data[7] * sin_a + _data[8] * cos_a;
	_data[7] = t;
}

template <class _ST>
void mat3<_ST>::from_euler(_ST x_angle, _ST y_angle, _ST z_angle)
{
	_ST cr = cos(x_angle);
	_ST sr = sin(x_angle);
	_ST cp = cos(y_angle);
	_ST sp = sin(y_angle);
	_ST cy = cos(z_angle);
	_ST sy = sin(z_angle);

	_data[0] = cp * cy;
	_data[1] = cp * sy;
	_data[2] = -sp;

	_ST srsp = sr * sp;
	_ST crsp = cr * sp;

	_data[3] = srsp * cy - cr * sy;
	_data[4] = srsp * sy + cr * cy;
	_data[5] = sr * cp;

	_data[6] = crsp * cy + sr * sy;
	_data[7] = crsp * sy - sr * cy;
	_data[8] = cr * cp;
}

template <class _ST>
void mat3<_ST>::to_euler(_ST& x_angle, _ST& y_angle, _ST& z_angle)
{
	y_angle = -asin(_data[2]);
	_ST cy = cos(y_angle);

	float rotx, roty;

	if(fcmp_gt(fabs(cy), _ST(0)))
	{
		_ST inv_c = _ST(1) / cy;
		rotx = _data[8] * inv_c;
		roty = _data[5] * inv_c;
		x_angle = atan2(roty, rotx);
		rotx = _data[0] * inv_c;
		roty = _data[1] * inv_c;
		z_angle = atan2(roty, rotx);
	}
	else
	{
		x_angle = _ST(0);
		rotx = _data[4];
		roty = -_data[3];
		z_angle = atan2(roty, rotx);
	}

	if(x_angle < 0.0) x_angle += _ST(TWO_PI);
	if(y_angle < 0.0) y_angle += _ST(TWO_PI);
	if(z_angle < 0.0) z_angle += _ST(TWO_PI);
}

template <class _ST>
void mat3<_ST>::scale(_ST s)
{
	_data[0] *= s;
	_data[1] *= s;
	_data[2] *= s;

	_data[3] *= s;
	_data[4] *= s;
	_data[5] *= s;

	_data[6] *= s;
	_data[7] *= s;
	_data[8] *= s;
}

template <class _ST>
void mat3<_ST>::scale(_ST _x, _ST _y, _ST _z)
{
	_data[0] *= _x;
	_data[1] *= _y;
	_data[2] *= _z;

	_data[3] *= _x;
	_data[4] *= _y;
	_data[5] *= _z;

	_data[6] *= _x;
	_data[7] *= _y;
	_data[8] *= _z;
}

template <class _ST>
void mat3<_ST>::scale(const vec3<_ST>& s)
{
	_data[0] *= s.x;
	_data[1] *= s.y;
	_data[2] *= s.z;

	_data[3] *= s.x;
	_data[4] *= s.y;
	_data[5] *= s.z;

	_data[6] *= s.x;
	_data[7] *= s.y;
	_data[8] *= s.z;
}

template <class _ST>
void mat3<_ST>::set_rotation(_ST angle, _ST _x, _ST _y, _ST _z)
{
	_ST cos_a = cos(angle);
	_ST inv_cos_a = _ST(1) - cos_a;
	_ST inv_cos_a_xy = inv_cos_a * _x * _y;
	_ST inv_cos_a_xz = inv_cos_a * _x * _z;
	_ST inv_cos_a_yz = inv_cos_a * _y * _z;
	_ST sin_a = sin(angle);
	_ST sin_a_x = sin_a * _x;
	_ST sin_a_y = sin_a * _y;
	_ST sin_a_z = sin_a * _z;

	_data[0] = cos_a + inv_cos_a * _x * _x;
	_data[3] = inv_cos_a_xy - sin_a_z;
	_data[6] = inv_cos_a_xz + sin_a_y;
	_data[1] = inv_cos_a_xy + sin_a_z;
	_data[4] = cos_a + inv_cos_a * _y * _y;
	_data[7] = inv_cos_a_yz - sin_a_x;
	_data[2] = inv_cos_a_xz - sin_a_y;
	_data[5] = inv_cos_a_yz + sin_a_x;
	_data[8] = cos_a + inv_cos_a * _z * _z;
}

template <class _ST>
inline
void mat3<_ST>::set_rotation(_ST angle, const vec3<_ST>& axis)
{
	set_rotation(angle, axis.x, axis.y, axis.z);
}

template <class _ST>
void mat3<_ST>::set_rotation_x(_ST angle)
{
	_ST cos_a = cos(angle);
	_ST sin_a = sin(angle);

	row0.set(_ST(1), _ST(0), _ST(0));
	row1.set(_ST(0), cos_a, sin_a);
	row2.set(_ST(0), -sin_a, cos_a);
}

template <class _ST>
void mat3<_ST>::set_rotation_y(_ST angle)
{
	_ST cos_a = cos(angle);
	_ST sin_a = sin(angle);

	row0.set(cos_a, _ST(0), -sin_a);
	row1.set(_ST(0), _ST(1), _ST(0));
	row2.set(sin_a, _ST(0), cos_a);
}

template <class _ST>
void mat3<_ST>::set_rotation_z(_ST angle)
{
	_ST cos_a = cos(angle);
	_ST sin_a = sin(angle);

	row0.set(cos_a, sin_a, _ST(0));
	row1.set(-sin_a, cos_a, _ST(0));
	row2.set(_ST(0), _ST(0), _ST(1));
}

template <class _ST>
void mat3<_ST>::set_scale(_ST s)
{
	row0.set(s, _ST(0), _ST(0));
	row1.set(_ST(0), s, _ST(0));
	row2.set(_ST(0), _ST(0), s);
}

template <class _ST>
void mat3<_ST>::set_scale(_ST _x, _ST _y, _ST _z)
{
	row0.set(_x, _ST(0), _ST(0));
	row1.set(_ST(0), _y, _ST(0));
	row2.set(_ST(0), _ST(0), _z);
}

template <class _ST>
void mat3<_ST>::set_scale(const vec3<_ST>& s)
{
	row0.set(s.x, _ST(0), _ST(0));
	row1.set(_ST(0), s.y, _ST(0));
	row2.set(_ST(0), _ST(0), s.z);
}

// implementation functions

template <class _ST>
inline
void mat3<_ST>::_copy(const _ST* src)
{
	memcpy(this, src, sizeof(_ST) * el_count);
}

template <class _ST>
void mat3<_ST>::_transpose(_ST* dest, const _ST* src)
{
	dest[0] = src[0];
	dest[1] = src[3];
	dest[2] = src[6];

	dest[3] = src[1];
	dest[4] = src[4];
	dest[5] = src[7];

	dest[6] = src[2];
	dest[7] = src[5];
	dest[8] = src[8];
}

template <class _ST>
void mat3<_ST>::_inverse(_ST* dest, const _ST* src)
{
	// co-factors
	_ST cf0 = src[4] * src[8] - src[5] * src[7];
	_ST cf1 = - (src[3] * src[8] - src[5] * src[6]);
	_ST cf2 = src[3] * src[7] - src[4] * src[6];
	_ST cf3 = - (src[1] * src[8] - src[2] * src[7]);
	_ST cf4 = src[0] * src[8] - src[2] * src[6];
	_ST cf5 = - (src[0] * src[7] - src[1] * src[6]);
	_ST cf6 = src[1] * src[5] - src[2] * src[4];
	_ST cf7 = - (src[0] * src[5] - src[2] * src[3]);
	_ST cf8 = src[0] * src[4] - src[1] * src[3];

	// determinant
	_ST det = src[0] * cf0 + src[1] * cf1 + src[2] * cf2;
	_ST inv_det = _ST(1) / det;

	// inverse
	dest[0] = cf0 * inv_det;
	dest[1] = cf3 * inv_det;
	dest[2] = cf6 * inv_det;

	dest[3] = cf1 * inv_det;
	dest[4] = cf4 * inv_det;
	dest[5] = cf7 * inv_det;

	dest[6] = cf2 * inv_det;
	dest[7] = cf5 * inv_det;
	dest[8] = cf8 * inv_det;
}

template <class _ST>
void mat3<_ST>::_inverse_transpose(_ST* dest, const _ST* src)
{
	// co-factors
	_ST cf0 = src[4] * src[8] - src[5] * src[7];
	_ST cf1 = - (src[3] * src[8] - src[5] * src[6]);
	_ST cf2 = src[3] * src[7] - src[4] * src[6];
	_ST cf3 = - (src[1] * src[8] - src[2] * src[7]);
	_ST cf4 = src[0] * src[8] - src[2] * src[6];
	_ST cf5 = - (src[0] * src[7] - src[1] * src[6]);
	_ST cf6 = src[1] * src[5] - src[2] * src[4];
	_ST cf7 = - (src[0] * src[5] - src[2] * src[3]);
	_ST cf8 = src[0] * src[4] - src[1] * src[3];

	// determinant
	_ST det = src[0] * cf0 + src[1] * cf1 + src[2] * cf2;
	_ST inv_det = _ST(1) / det;

	// transpose of the inverse
	dest[0] = cf0 * inv_det;
	dest[1] = cf1 * inv_det;
	dest[2] = cf2 * inv_det;

	dest[3] = cf3 * inv_det;
	dest[4] = cf4 * inv_det;
	dest[5] = cf5 * inv_det;

	dest[6] = cf6 * inv_det;
	dest[7] = cf7 * inv_det;
	dest[8] = cf8 * inv_det;
}

template <class _ST>
void mat3<_ST>::_adjoint(_ST* dest, const _ST* src)
{
	// co-factors
	dest[0] = src[4] * src[8] - src[5] * src[7];
	dest[1] = - (src[1] * src[8] - src[2] * src[7]);
	dest[2] = src[1] * src[5] - src[2] * src[4];
	dest[3] = - (src[3] * src[8] - src[5] * src[6]);
	dest[4] = src[0] * src[8] - src[2] * src[6];
	dest[5] = - (src[0] * src[5] - src[2] * src[3]);
	dest[6] = src[3] * src[7] - src[4] * src[6];
	dest[7] = - (src[0] * src[7] - src[1] * src[6]);
	dest[8] = src[0] * src[4] - src[1] * src[3];
}

template <class _ST>
inline
void mat3<_ST>::_swap(_ST& a, _ST& b)
{
	_ST t = a;
	a = b;
	b = t;
}


// matrix operations; global functions

// WARNING: result must not be the same object as one of the operands
template <class _ST>
void mul(mat3<_ST>& r, const mat3<_ST>& a, const mat3<_ST>& b)
{
	r(0) = a(0) * b(0) + a(1) * b(3) + a(2) * b(6);
	r(1) = a(0) * b(1) + a(1) * b(4) + a(2) * b(7);
	r(2) = a(0) * b(2) + a(1) * b(5) + a(2) * b(8);

	r(3) = a(3) * b(0) + a(4) * b(3) + a(5) * b(6);
	r(4) = a(3) * b(1) + a(4) * b(4) + a(5) * b(7);
	r(5) = a(3) * b(2) + a(4) * b(5) + a(5) * b(8);

	r(6) = a(6) * b(0) + a(7) * b(3) + a(8) * b(6);
	r(7) = a(6) * b(1) + a(7) * b(4) + a(8) * b(7);
	r(8) = a(6) * b(2) + a(7) * b(5) + a(8) * b(8);
}

template <class _ST>
void add(mat3<_ST>& r, const mat3<_ST>& a, const mat3<_ST>& b)
{
	r(0) = a(0) + b(0);
	r(1) = a(1) + b(1);
	r(2) = a(2) + b(2);

	r(3) = a(3) + b(3);
	r(4) = a(4) + b(4);
	r(5) = a(5) + b(5);

	r(6) = a(6) + b(6);
	r(7) = a(7) + b(7);
	r(8) = a(8) + b(8);
}


} // namespace math3d


#endif //_MATH3D_MAT3_H_
