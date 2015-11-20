#pragma once
//#include <iostream>
#include "Vector2D.h"
#include "Rect2D.h"
#include "Segment2D.h"
#include <windef.h>
#include <string>

using namespace std;

template <typename TYPE>
class Circle2D
{
private:
	Vector2D<TYPE> _center;
	TYPE _radius;

public:
	inline TYPE GetRadius() const { return _radius; }

	inline Vector2D<TYPE> GetCenter() const { return _center; }

	inline TYPE GetLeft() const { return _center.x - _radius; }
	inline TYPE GetTop() const { return _center.y - _radius; }
	inline TYPE GetRight() const { return _center.x + _radius; }
	inline TYPE GetBottom() const { return _center.y + _radius; }

	inline Circle2D(Vector2D<TYPE> center, TYPE radius) : _center(center), _radius(radius) 	{}

	inline Rect2D<TYPE> GetBoundBox()
	{
		Rect2Df rect(GetLeft(), GetTop(), GetRight(), GetBottom());
		return rect;
	}

	inline bool IsPointInside(Vector2D<TYPE> inPoint)
	{
		Vector2D<TYPE> link = inPoint - _center;
		return link.LengthSq() <= _radius * _radius;
	}

	wstring ToString(int inCountAfterPoint = MAXINT32)
	{
		return _center.ToString(inCountAfterPoint) + L"; radius: " + CutAfterPoint(to_wstring(_radius), inCountAfterPoint);
	}
};

typedef Circle2D<float> Circle2Df;