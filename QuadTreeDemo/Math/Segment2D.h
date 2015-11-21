#pragma once
#include "Vector2D.h"
#include "CommonMath.h"
#include <windef.h>
#include <string>

using namespace std;

template <typename TYPE>
class Segment2D
{
private:
	Vector2D<TYPE> _origin;
	Vector2D<TYPE> _direction;
	TYPE	_length;
	TYPE	_invlength;

public:

	Segment2D(): _origin(0, 0), _direction(0, 0), _length(0), _invlength(0) {}

	Segment2D(const Vector2D<TYPE>& inOrigin, const Vector2D<TYPE>& inDestination)
	{
		SetParam(inOrigin, inDestination);
	}

	void SetLength(TYPE inLength)
	{
		_length = inLength;
		if (!IsEqualReal(_length, 0))
			_invlength = 1 / _length;
		else
		{
			_length = 0;
			_invlength = 0;
		}
	}

	void Move(const Vector2D<TYPE>& inOrigin) { _origin = inOrigin; }

	void ChangeOrigin(const Vector2D<TYPE>& inOrigin)
	{
		Vector2D<TYPE> olddest = GetDestination();
		SetParam(inOrigin, olddest);
	}

	void ChangeDestination(const Vector2D<TYPE>& inDest)
	{
		SetParam(_origin, inDest);
	}

	void SetParam(const Vector2D<TYPE>& inOrigin, const Vector2D<TYPE>& inDestination)
	{
		_origin = inOrigin;
		_direction = inDestination - inOrigin;
		SetLength(_direction.Length());
		_direction *= _invlength;
	}

	bool IsDefine() const { return !IsEqualReal(_length, 0); }
	TYPE GetLength() const { return _length; }
	TYPE GetInvLength() const { return _invlength; }

	const Vector2D<TYPE>& GetOrigin() const { return _origin; }
	const Vector2D<TYPE>& GetDirection() const { return _direction; }
	Vector2D<TYPE> GetFullLengthDirection() const { return _direction * _length; }
	Vector2D<TYPE> GetDestination() const { return _origin + _direction * _length; }

	/*wstring ToString(int inCountAfterPoint = MAXINT32)
	{
		return _center.ToString(inCountAfterPoint) + L"; radius: " + CutAfterPoint(to_wstring(_radius), inCountAfterPoint);
	}*/
};

typedef Segment2D<float> Segment2Df;
