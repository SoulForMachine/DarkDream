
#ifndef _BASELIB_COMMON_H_
#define _BASELIB_COMMON_H_

#pragma warning(disable : 4996) // 'function': was declared deprecated
#pragma warning(disable : 4251) // class 'x' needs to have dll-interface to be used by clients of class 'y'

#ifdef BASELIB_EXPORTS

#define BASELIB_API __declspec(dllexport)

#else

#define BASELIB_API __declspec(dllimport)

#endif

// number of elements in array a
#define COUNTOF(a) (sizeof(a) / sizeof(*(a)))

template <class _Type>
inline
_Type RoundUp(_Type n, size_t a)
{
	return _Type( (size_t(n) + (a - 1)) & ~(a - 1) );
}

inline 
bool IsPow2(size_t num)
{
	return !((num - 1) & num);
}

/*
	Returns next power of two, or the same number if it's already power of two
*/
template <class _Type>
inline
_Type RoundToNextPow2(_Type n)
{
	_Type result = 1;
	while(result < n)
		result <<= 1;
	return result;
}

template <class _Type>
inline
_Type Min(_Type a, _Type b)
{
	return (a < b)? a: b;
}

template <class _Type>
inline
_Type Max(_Type a, _Type b)
{
	return (a > b)? a: b;
}


#endif // _BASELIB_COMMON_H_
