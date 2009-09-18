
/* ----------------------------------------
	File: mathutil.h
	Purpose: 3d math library helpers
	Author: Milan D.
	Date: 01.05.2005
   ---------------------------------------- */


#ifndef _MATH3D_MATHUTIL_H_
#define _MATH3D_MATHUTIL_H_

#ifdef _MSC_VER
//	#pragma warning(disable: 4244) // floating conversion warning
#endif

#include <cmath>
#include <cfloat>



namespace math3d
{

// some constants
const float PI = float(3.14159265358979323846264338327950288419716939937510582);
const float TWO_PI	= 2.0f * PI;
const float HALF_PI = 0.5f * PI;


//const float FLT_EPSILON = 1e-4F;
//const double DBL_EPSILON = 1e-10;
//const long double LDBL_EPSILON = 1e-10L;



/*
	this template function has no meaning for
	non-floating types, so the default template
	is not defined, and specializations are provided
	for every floating type
*/
template <class _ST>
inline _ST epsilon();

/*
	specializations for every floating type
	that return an apropriate epsilon values
*/
template <> inline float epsilon<float>(){ return FLT_EPSILON; }
template <> inline double epsilon<double>(){ return DBL_EPSILON; }
template <> inline long double epsilon<long double>(){ return LDBL_EPSILON; }


/*
	functions for comparing floating types
*/
template <class _ST>
inline bool fcmp_eq(_ST t1, _ST t2, _ST err = epsilon<_ST>())
{
	return fabs(t1 - t2) <= err;
}
template <class _ST>
inline bool fcmp_ne(_ST t1, _ST t2, _ST err = epsilon<_ST>())
{
	return fabs(t1 - t2) > err;
}
template <class _ST>
inline bool fcmp_lt(_ST t1, _ST t2, _ST err = epsilon<_ST>())
{
	return (t1 - t2) < err;
}
template <class _ST>
inline bool fcmp_le(_ST t1, _ST t2, _ST err = epsilon<_ST>())
{
	return (t1 - t2) <= err;
}
template <class _ST>
inline bool fcmp_gt(_ST t1, _ST t2, _ST err = epsilon<_ST>())
{
	return (t1 - t2) > err;
}
template <class _ST>
inline bool fcmp_ge(_ST t1, _ST t2, _ST err = epsilon<_ST>())
{
	return (t1 - t2) >= err;
}

/*
	convert an angle from radians to degrees
	and vice versa
*/
template <class _ST>
inline _ST rad2deg(_ST rad) { return rad * _ST(180 / PI); }
template <class _ST>
inline _ST deg2rad(_ST deg) { return deg * _ST(PI / 180); }

/*
	trigonometric functions that work with
	angles in degrees rather than radians
*/
template <class _ST>
inline _ST deg_sin(_ST angle) { return sin(deg2rad(angle)); }
template <class _ST>
inline _ST deg_cos(_ST angle) { return cos(deg2rad(angle)); }
template <class _ST>
inline _ST deg_tan(_ST angle) { return tan(deg2rad(angle)); }
template <class _ST>
inline _ST deg_asin(_ST t) { return rad2deg(asin(t)); }
template <class _ST>
inline _ST deg_acos(_ST t) { return rad2deg(acos(t)); }
template <class _ST>
inline _ST deg_atan(_ST t) { return rad2deg(atan(t)); }

/*
	set t to be in range low <= t <= high;
	assumes that low < high 
*/
template <class _ST>
inline void clamp(_ST& t, _ST low, _ST high)
{
	if(t < low) t = low;
	else if(t > high) t = high;
}

template <class _ST>
_ST frand(int min, int max)
{
	if(min >= max) return _ST(0);
	int integ = min + rand() % (max - min);
	_ST fract = _ST(rand() % 10001) / _ST(9999);
	return integ + fract;
}

template <class _ST>
inline
_ST lerp(_ST start, _ST end, _ST factor)
{
	return start + (end - start) * factor;
}

template <class _T, class _ST>
void lerp(_T& result, const _T& start, const _T& end, _ST factor)
{
	_T temp;
	sub(temp, end, start);
	mul(temp, temp, factor);
	add(result, start, temp);
}

/*
	Round to nearest integer
*/
template <class _ST>
inline
_ST round(_ST val)
{
	return (float)(int)(val + _ST(0.5));
}

/*
	Return fractional part of val
*/
template <class _ST>
inline
_ST frac(_ST val)
{
	return val - (float)(int)val;
}


} // namespace math3d



#endif // _MATH3D_MATHUTIL_H_
