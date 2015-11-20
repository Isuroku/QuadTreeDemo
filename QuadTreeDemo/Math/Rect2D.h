#pragma once
#include <windef.h>
#include "Vector2D.h"
#include "CommonMath.h"

using namespace std;

template <typename TYPE>
class Rect2D
{
private:
	TYPE _left;
	TYPE _top;
	TYPE _right;
	TYPE _bottom;

public:
	TYPE GetLeft() const { return _left; }
	TYPE GetTop() const { return _top; }
	TYPE GetRight() const { return _right; }
	TYPE GetBottom() const { return _bottom; }

	TYPE GetWidth() const { return _right - _left; }
	TYPE GetHeight() const { return _bottom - _top; }

	Rect2D<TYPE>(const Rect2D<TYPE>& source)
	{
		_left = source._left;
		_top = source._top;
		_right = source._right;
		_bottom = source._bottom;
	}

	Rect2D<TYPE>(TYPE left, TYPE top, TYPE right, TYPE bottom)
	{
		_left = left;
		_top = top;
		_right = right;
		_bottom = bottom;
	}

	Rect2D<TYPE>(Vector2D<TYPE> inLeftUp, TYPE Width, TYPE Height)
	{
		_left = inLeftUp.x;
		_top = inLeftUp.y;
		_right = _left + Width;
		_bottom = _top + Height;
	}

	/*public Rect2Df(Circle2Df circle, bool in_bSphereInRect)
	{
		float a = circle.radius;
		if (!in_bSphereInRect)
			a = (float)Math.Sqrt((circle.radius * circle.radius) / 2);

		_left = circle.center.x - a;
		_top = circle.center.y - a;
		_right = circle.center.x + a;
		_bottom = circle.center.y + a;
	}*/

	Vector2D<TYPE> GetOrigin() const
	{
		return Vector2D<TYPE>((_left + _right) / 2, (_top + _bottom) / 2);
	}

	/*public OBB2D GetOBB()
	{
		return new OBB2D((_left + _right) / 2, (_top + _bottom) / 2, width, height);
	}*/

	void UpdateMin(Vector2D<TYPE> u)
	{
		if (_left > u.x) _left = u.x;
		if (_top > u.y) _top = u.y;
	}

	void UpdateMax(Vector2D<TYPE> u)
	{
		if (_right < u.x) _right = u.x;
		if (_bottom < u.y) _bottom = u.y;
	}

	void Bound(TYPE left, TYPE top, TYPE right, TYPE bottom)
	{
		_left = ClipNumber(_left, left, right);
		_top = ClipNumber(_top, top, bottom);
		_right = ClipNumber(_right, left, right);
		_bottom = ClipNumber(_bottom, top, bottom);
	}

	RECT GetWinRect() const
	{
		RECT rct;
		rct.left = static_cast<LONG>(_left);
		rct.top = static_cast<LONG>(_top);
		rct.right = static_cast<LONG>(_right);
		rct.bottom = static_cast<LONG>(_bottom);
		return rct;
	}

	//inline Vector2D<TYPE> operator [](int id)
	inline Vector2D<TYPE> GetCorner(int id) const
	{
		Vector2D<TYPE> res;
		switch (id)
		{
			case 0: res.x = _left; res.y = _top; break;
			case 1: res.x = _right; res.y = _top; break;
			case 2: res.x = _right; res.y = _bottom; break;
			case 3: res.x = _left; res.y = _bottom; break;
			default: throw std::invalid_argument("index error");
		}
		return res;
	}
};

typedef Rect2D<float> Rect2Df;
typedef Rect2D<unsigned short> Rect2Dus;
typedef Rect2D<unsigned int> Rect2Dui;