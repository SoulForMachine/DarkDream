
#ifndef _BASELIB_RECT_H_
#define _BASELIB_RECT_H_


template <class _Type>
struct Rect
{
	_Type x1, y1, x2, y2;

	Rect(){}
	Rect(_Type _x1, _Type _y1, _Type _x2, _Type _y2)
	{
		x1 = _x1; y1 = _y1;
		x2 = _x2; y2 = _y2;
	}

	_Type Width() const
	{
		return x2 - x1;
	}

	_Type Height() const
	{
		return y2 - y1;
	}

	void SetWidth(_Type width)
	{
		x2 = x1 + width;
	}

	void SetHeight(_Type height)
	{
		y2 = y1 + height;
	}

	void SetPosition(_Type x, _Type y)
	{
		_Type width = Width();
		_Type height = Height();
		x1 = x;
		y1 = y;
		x2 = x1 + width;
		y2 = y1 + height;
	}

	bool IsPointInside(_Type x, _Type y) const
	{
		return x >= x1 && x <= x2 && y >= y1 && y <= y2;
	}

	void Offset(_Type x, _Type y)
	{
		x1 += x;
		x2 += x;
		y1 += y;
		y2 += y;
	}

	void Merge(const Rect& rect)
	{
		x1 = Min(x1, rect.x1);
		x2 = Max(x2, rect.x2);
		y1 = Min(y1, rect.y1);
		y2 = Max(y2, rect.y2);
	}

	void Resize(_Type x, _Type y)
	{
		x2 += x;
		y2 += y;
	}

	void Center(Rect& rect)
	{
		_Type w = rect.Width();
		_Type h = rect.Height();

		rect.x1 = x1 + (Width() - w) / _Type(2);
		rect.x2 = rect.x1 + w;
		rect.y1 = y1 + (Height() - h) / _Type(2);
		rect.y2 = rect.y1 + h;
	}
};


typedef Rect<int> RectInt;
typedef Rect<float> RectFloat;


#endif // _BASELIB_RECT_H_
