#pragma once

#include <string>
#include "../str.h"

using namespace std;

template<typename TYPE>
class	Vector2D
{
public:
	union
	{
		struct
		{
			TYPE	x, y;
		};
		TYPE v[2];
	};

	Vector2D() {}
	explicit Vector2D(const TYPE& v) { y = x = v; }
	Vector2D(const Vector2D<TYPE>& v) { x = v.x; y = v.y; }
	Vector2D(const TYPE& vx, const TYPE& vy) { x = vx; y = vy; }
	Vector2D(CPoint inPnt)  { x = static_cast<TYPE>(inPnt.x); y = static_cast<TYPE>(inPnt.y); }

	template<typename TYPE2>
	explicit Vector2D(const Vector2D<TYPE2>& v) { x = static_cast<TYPE>(v.x); y = static_cast<TYPE>(v.y); }

	inline TYPE operator [](int id) { return v[id]; }

	inline Vector2D<TYPE> operator += (const Vector2D<TYPE>& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	inline Vector2D<TYPE> operator -= (const Vector2D<TYPE>& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	inline Vector2D<TYPE> operator *= (const TYPE& f)
	{
		x *= f;
		y *= f;
		return *this;
	}

	inline Vector2D<TYPE> operator /= (const TYPE& f)
	{
		TYPE ff = 1 / f;
		x *= ff;
		y *= ff;
		return *this;
	}

	TYPE	LengthSq() const
	{
		return static_cast<TYPE>(x*x + y*y);
	}

	TYPE	Length() const
	{
		return static_cast<TYPE>(sqrt(LengthSq()));
	}

	inline Vector2D<TYPE> Normalize()
	{
		TYPE tInv = TYPE(1) / Length();
		x *= tInv;
		y *= tInv;
		return *this;
	}

	inline TYPE NormalizeL()
	{
		TYPE tLen = Length();
		TYPE tInv = TYPE(1) / tLen;
		x *= tInv;
		y *= tInv;
		return tLen;
	}

	inline void zero()
	{
		x = TYPE(0);
		y = TYPE(0);
	}

	inline Vector2D<TYPE> &RotateVector(TYPE angle)
	{
		float cosA, sinA;
		SinCos(angle, sinA, cosA);
		float _x = x * cosA - y * sinA;
		y = x * sinA + y * cosA;
		x = _x;
		return *this;
	}

	bool operator == (const Vector2D<TYPE> &v) const
	{
		return (x == v.x) && (y == v.y);
	}

	bool operator !=(const Vector2D<TYPE> &v) const {
		return (x != v.x) || (y != v.y);
	}

	void ShiftX(TYPE shift) { x += shift; }
	void ShiftY(TYPE shift) { y += shift; }
	void Shift(TYPE shiftX, TYPE shiftY) { x += shiftX; y += shiftY; }

	wstring ToString(int inCountAfterPoint = MAXINT32)
	{
		return L"x: " + CutAfterPoint(to_wstring(x), inCountAfterPoint) + L"; y: " + CutAfterPoint(to_wstring(y), inCountAfterPoint);
	}
};

typedef Vector2D<float> Vector2Df;
typedef Vector2D<unsigned short> Vector2Dus;
typedef Vector2D<unsigned int> Vector2Dui;
typedef Vector2D<int> Vector2Di;

// --- Vector2D operators ---
template<typename TYPE>
inline Vector2D<TYPE> operator *(const Vector2D<TYPE>& v, const TYPE& a)
{
	return Vector2D<TYPE>(v.x*a, v.y*a);
}

template<typename TYPE>
inline Vector2D<TYPE> operator *(const TYPE& a, const Vector2D<TYPE>& v)
{
	return Vector2D<TYPE>(v.x*a, v.y*a);
}

template<typename TYPE>
inline Vector2D<TYPE> operator -(const Vector2D<TYPE>& v1, const Vector2D<TYPE>& v2)
{
	return Vector2D<TYPE>(v1.x - v2.x, v1.y - v2.y);
}

template<typename TYPE>
inline Vector2D<TYPE> operator -(const Vector2D<TYPE>& v1)
{
	return Vector2D<TYPE>(-v1.x, -v1.y);
}

template<typename TYPE>
inline Vector2D<TYPE> operator +(const Vector2D<TYPE>& v1, const Vector2D<TYPE>& v2)
{
	return Vector2D<TYPE>(v1.x + v2.x, v1.y + v2.y);
}

template<typename TYPE>
inline Vector2D<TYPE> Normalize(const Vector2D<TYPE>& v)
{
	TYPE tInv = TYPE(1) / v.Length();
	return v*tInv;
}

template<typename TYPE>
inline Vector2D<TYPE> Perpendicular(const Vector2D<TYPE>& v)
{
	return Vector2D<TYPE>(-v.y, v.x);
}

template<typename TYPE>
inline TYPE Length(const Vector2D<TYPE>& v)
{
	return v.Length();
}

template<typename TYPE>
inline TYPE SqrLen(const Vector2D<TYPE> &v)
{
	return v.x*v.x + v.y*v.y;
}

template<typename TYPE>
inline TYPE SqrLen(const Vector2D<TYPE> &v1, const Vector2D<TYPE> &v2)
{
	TYPE x = v2.x - v1.x;
	TYPE y = v2.y - v1.y;
	return x*x + y*y;
}

template <typename TYPE>
inline TYPE DotProduct(const Vector2D<TYPE> &v1, const Vector2D<TYPE> &v2)
{
	return v1.x*v2.x + v1.y*v2.y;
}

template<typename TYPE>
inline Vector2D<TYPE> scale(const Vector2D<TYPE>& a, const Vector2D<TYPE>& b)
{
	return Vector2D<TYPE>(a.x * b.x, a.y * b.y);
}