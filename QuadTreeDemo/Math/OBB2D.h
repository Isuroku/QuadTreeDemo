#pragma once
#include "Vector2D.h"
#include "Matrix2x2.h"
#include "Segment2D.h"
#include "Rect2D.h"

template <typename TYPE>
class OBB2D
{
public:

	inline OBB2D(Vector2D<TYPE> inOrigin, TYPE inWidth, TYPE inHeight) : _origin(inOrigin), _halfsize(inWidth / 2, inHeight / 2)
	{
		_orientation.one();
	}

	inline OBB2D(TYPE inPosX, TYPE inPosY, TYPE inWidth, TYPE inHeight) : _origin(inPosX, inPosY), _halfsize(inWidth/2, inHeight/2)
	{
		_orientation.one();
	}

	inline Vector2D<TYPE> GetOrigin() const { return _origin; }
	inline Matrix2x2<TYPE> GetOrientation() const { return _orientation; }
	inline Vector2D<TYPE> GetHalfSizes() const { return _halfsize; }

	inline void SetRotation(TYPE inAngle) { _orientation.Rotate(in_Angle); }

	inline void Rotate(TYPE inAngle)
	{
		Matrix2x2<TYPE> rot = new Matrix2x2<TYPE>();
		rot.Rotate(in_Angle);
		_orientation = _orientation * rot;
	}

	inline Rect2D<TYPE> GetBoundBox() const
	{
		float fShX = abs(_orientation.m11 * _halfsize.x) + abs(_orientation.m21 * _halfsize.y);
		float fShY = abs(_orientation.m21 * _halfsize.x) + abs(_orientation.m22 * _halfsize.y);
		Rect2Df bb(_origin.x - fShX, _origin.y - fShY, _origin.x + fShX, _origin.y + fShY);
		return bb;
	}

	inline void GetVertices(Vector2D<TYPE>* vertarray) const
	{
		Vector2D<TYPE> avEAxis[2] { _halfsize.x * _orientation.forward, _halfsize.y * _orientation.up };

		vertarray[0] = _origin - avEAxis[0] - avEAxis[1];
		vertarray[1] = _origin + avEAxis[0] - avEAxis[1];
		vertarray[2] = _origin + avEAxis[0] + avEAxis[1];
		vertarray[3] = _origin - avEAxis[0] + avEAxis[1];
	}

	//inline Vector2D<TYPE> operator [](int id)
	inline Vector2D<TYPE> GetCorner(int id) const
	{ 
		switch (id)
		{
			case 0: return _origin - avEAxis[0] - avEAxis[1];
			case 1: return _origin + avEAxis[0] - avEAxis[1];
			case 2: return _origin + avEAxis[0] + avEAxis[1];
			case 3: return _origin - avEAxis[0] + avEAxis[1];
		}
		throw std::invalid_argument("index error");
	}

	bool IsPointInside(const Vector2D<TYPE>& inPoint) const
	{
		Vector2D<TYPE> vDiff = inPoint - _origin;
		Vector2D<TYPE> vProjection(DotProduct(_orientation.forward, vDiff), DotProduct(_orientation.up, vDiff));

		return fabs(vProjection.x) <= _halfsize.x && fabs(vProjection.y) <= _halfsize.y;
	}

	wstring ToString(int inCountAfterPoint = MAXINT32)
	{
		return L"origin: " + CutAfterPoint(to_wstring(_origin.x), inCountAfterPoint) + L" " + CutAfterPoint(to_wstring(_origin.y), inCountAfterPoint) +
			L"; halfsize: " + CutAfterPoint(to_wstring(_halfsize.x), inCountAfterPoint) + L" " + CutAfterPoint(to_wstring(_halfsize.y), inCountAfterPoint) +
			L"; orientation: " + CutAfterPoint(to_wstring(acosf(_orientation.m11) / 3.1415f * 180), inCountAfterPoint);
	}
		
private:
	Vector2D<TYPE> _origin;
	Matrix2x2<TYPE> _orientation;
	Vector2D<TYPE> _halfsize;
};

typedef OBB2D<float> OBB2Df;

