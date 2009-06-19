
#ifndef _RECT_H_
#define _RECT_H_

template <class _T>
struct Rect
{
	_T x1, y1, x2, y2;
};

typedef Rect<int> RectInt;
typedef Rect<float> RectFloat;


#endif // _RECT_H_
