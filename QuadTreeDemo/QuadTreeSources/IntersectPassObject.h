#pragma once
#include "../Math//Vector2D.h"

struct SIntersectPassObject
{
	Vector2Dus	Start;
	Vector2Dus  End;
	Vector2Dus	Current;
	double      T;
	double      Kx;
	double      Ky;
	bool        InfinityX;
	bool        InfinityY;
	bool        ForwardX;
	bool        ForwardY;

	SIntersectPassObject(const Vector2Df& inStart, const Vector2Df& inEnd, float inWORDperMeter):
		T(0), Kx(0), Ky(0), InfinityX(false), InfinityY(false), ForwardX(true), ForwardY(true)
	{
		Start.x = static_cast<unsigned short>(inStart.x * inWORDperMeter);
		Start.y = static_cast<unsigned short>(inStart.y * inWORDperMeter);
		End.x = static_cast<unsigned short>(inEnd.x * inWORDperMeter);
		End.y = static_cast<unsigned short>(inEnd.y * inWORDperMeter);

		Current = Start;
		if (End.x == Start.x)
			InfinityX = true;
		else
			Kx = 1 / (double)(End.x - Start.x);

		if (Kx < 0)
		{
			ForwardX = false;
			Kx *= -1;
		}

		if (End.y == Start.y)
			InfinityY = true;
		else
			Ky = 1 / (double)(End.y - Start.y);

		if (Ky < 0)
		{
			ForwardY = false;
			Ky *= -1;
		}
	}
};
