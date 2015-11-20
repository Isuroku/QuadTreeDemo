#include "stdafx.h"
#include "Intersect.h"
#include <cmath>

// Ripped from Magic Software

bool TestIntersectionBoxSphere(const OBB2Df& Box, const Circle2Df& Circle)
{
	{
		// Test for intersection in the coordinate system of the box by
		// transforming the sphere into that coordinate system.
		Vector2Df kCDiff = Circle.GetCenter() - Box.GetOrigin();

		float fAx = abs(DotProduct(kCDiff, Box.GetOrientation().forward));
		float fAy = abs(DotProduct(kCDiff, Box.GetOrientation().up));

		float fDx = fAx - Box.GetHalfSizes().x;
		float fDy = fAy - Box.GetHalfSizes().y;

		if (fAx <= Box.GetHalfSizes().x)
		{
			if (fAy <= Box.GetHalfSizes().y)
			{
				// circle center inside box
				return true;
			}
			else
			{
				// potential circle-edge intersection with edge formed by faces y
				return fDy <= Circle.GetRadius();
			}
		}
		else
		{
			if (fAy <= Box.GetHalfSizes().y)
			{
				// potential circle-edge intersection with edge formed by faces x
				return fDx <= Circle.GetRadius();
			}
			else
			{
				float fRSqr = Circle.GetRadius() * Circle.GetRadius();
				return fDx * fDx + fDy * fDy <= fRSqr;
			}
		}
	}
}

bool TestIntersectionBoxSphere(const Rect2Df& BoundBox, const Circle2Df& Circle)
{
	// Test for intersection in the coordinate system of the box by
	// transforming the sphere into that coordinate system.
	Vector2Df kCDiff = Circle.GetCenter() - BoundBox.GetOrigin();

	float fAx = abs(kCDiff.x);
	float fAy = abs(kCDiff.y);

	float BBHalfX = BoundBox.GetWidth() / 2;
	float BBHalfY = BoundBox.GetHeight() / 2;

	float fDx = fAx - BBHalfX;
	float fDy = fAy - BBHalfY;

	if (fAx <= BBHalfX)
	{
		if (fAy <= BBHalfY)
		{
			// circle center inside box
			return true;
		}
		else
		{
			// potential circle-edge intersection with edge formed by faces y
			return fDy <= Circle.GetRadius();
		}
	}
	else
	{
		if (fAy <= BBHalfY)
		{
			// potential circle-edge intersection with edge formed by faces x
			return fDx <= Circle.GetRadius();
		}
		else
		{
			float fRSqr = Circle.GetRadius() * Circle.GetRadius();
			return fDx * fDx + fDy * fDy <= fRSqr;
		}
	}
}

bool TestIntersectionBoxBox(const OBB2Df& Box0, const OBB2Df& Box1)
{
	// Cutoff for cosine of angles between box axes.  This is used to catch
	// the cases when at least one pair of axes are parallel.  If this happens,
	// there is no need to test for separation along the Cross(A[i],B[j])
	// directions.
	const float fCutoff = 0.999999f;
	bool bExistsParallelPair = false;
	int i;

	// convenience variables
	//const Vector3D<float>* akA = rkBox0.GetOrientation().lines;
	//const Vector3D<float>* akB = rkBox1.GetOrientation().lines;
	Matrix2x2f akA = Box0.GetOrientation();
	Matrix2x2f akB = Box1.GetOrientation();

	//const float* afEA = rkBox0.GetHalfSizes().v;
	//const float* afEB = rkBox1.GetHalfSizes().v;
	Vector2Df afEA = Box0.GetHalfSizes();
	Vector2Df afEB = Box1.GetHalfSizes();

	// compute difference of box centers, D = C1-C0
	Vector2Df kD = Box1.GetOrigin() - Box0.GetOrigin();

	float aafC[2][2];     // matrix C = A^T B, c_{ij} = Dot(A_i,B_j)
	float aafAbsC [2][2];  // |c_{ij}|
	float afAD[2];         // Dot(A_i,D)
	float fR0, fR1, fR;                  // interval radii and distance between centers
	float fR01;                          // = R0 + R1

										 // axis C0+t*A0
	for (i = 0; i < 2; i++)
	{
		aafC[0][i] = DotProduct(akA[0], akB[i]);
		aafAbsC[0][i] = abs(aafC[0][i]);
		if (aafAbsC[0][i] > fCutoff)
			bExistsParallelPair = true;
	}

	afAD[0] = DotProduct(akA[0], kD);
	fR = abs(afAD[0]);
	fR1 = afEB[0] * aafAbsC[0][0] + afEB[1] * aafAbsC[0][1];
	fR01 = afEA[0] + fR1;
	if (fR > fR01)
		return false;

	// axis C0+t*A1
	for (i = 0; i < 2; i++)
	{
		aafC[1][i] = DotProduct(akA[1], akB[i]);
		aafAbsC[1][i] = abs(aafC[1][i]);
		if (aafAbsC[1][i] > fCutoff)
			bExistsParallelPair = true;
	}

	afAD[1] = DotProduct(akA[1], kD);
	fR = abs(afAD[1]);
	fR1 = afEB[0] * aafAbsC[1][0] + afEB[1] * aafAbsC[1][1];
	fR01 = afEA[1] + fR1;
	if (fR > fR01)
		return false;

	// axis C0+t*B0
	fR = abs(DotProduct(akB[0], kD));
	fR0 = afEA[0] * aafAbsC[0][0] + afEA[1] * aafAbsC[1][0];
	fR01 = fR0 + afEB[0];
	if (fR > fR01)
		return false;

	// axis C0+t*B1
	fR = abs(DotProduct(akB[1], kD));
	fR0 = afEA[0] * aafAbsC[0][1] + afEA[1] * aafAbsC[1][1];
	fR01 = fR0 + afEB[1];
	if (fR > fR01)
		return false;

	// At least one pair of box axes was parallel, so the separation is
	// effectively in 2D where checking the "edge" normals is sufficient for
	// the separation of the boxes.
	if (bExistsParallelPair)
		return true;

	return true;
}




bool Clip(float in_fDenom, float in_fNumer, const Vector2Df& in_vNorm, float& io_rfT0, float& io_rfT1, Vector2Df& out_vNorm0, Vector2Df& out_vNorm1)
{
	// Return value is 'true' if line segment intersects the current test
	// plane.  Otherwise 'false' is returned in which case the line segment
	// is entirely clipped.

	if (in_fDenom > 0.0f)
	{
		if (in_fNumer > in_fDenom * io_rfT1)
			return false;
		if (in_fNumer > in_fDenom * io_rfT0)
		{
			io_rfT0 = in_fNumer / in_fDenom;
			out_vNorm0 = in_vNorm;
		}
		return true;
	}
	else if (in_fDenom < 0.0f)
	{
		if (in_fNumer > in_fDenom * io_rfT0)
			return false;
		if (in_fNumer > in_fDenom * io_rfT1)
		{
			io_rfT1 = in_fNumer / in_fDenom;
			out_vNorm1 = in_vNorm;
		}
		return true;
	}
	else
	{
		return in_fNumer <= 0.0f;
	}
}

bool IsIntersectSegment(const Vector2Df& in_vOrigin, const Vector2Df& in_vDirection,
	const Vector2Df& in_vOBBHalfSize, const Matrix2x2f& in_mOBBOrient,
	float& io_rfT0, float& io_rfT1, Vector2Df& out_vNorm0, Vector2Df& out_vNorm1)
{
	float fSaveT0 = io_rfT0, fSaveT1 = io_rfT1;

	bool bNotEntirelyClipped =
		Clip(+in_vDirection.x, -in_vOrigin.x - in_vOBBHalfSize.x, -1.f * in_mOBBOrient.forward, io_rfT0, io_rfT1, out_vNorm0, out_vNorm1) &&
		Clip(-in_vDirection.x, +in_vOrigin.x - in_vOBBHalfSize.x, in_mOBBOrient.forward, io_rfT0, io_rfT1, out_vNorm0, out_vNorm1) &&
		Clip(+in_vDirection.y, -in_vOrigin.y - in_vOBBHalfSize.y, -1.f * in_mOBBOrient.up, io_rfT0, io_rfT1, out_vNorm0, out_vNorm1) &&
		Clip(-in_vDirection.y, +in_vOrigin.y - in_vOBBHalfSize.y, in_mOBBOrient.up, io_rfT0, io_rfT1, out_vNorm0, out_vNorm1);

	/*bool bNotEntirelyClipped =
	Clip(+in_vDirection.x, -in_vOrigin.x - in_vOBBHalfSize.x, -1 * in_mOBBOrient.forward(), io_rfT0, io_rfT1, out_vNorm0, out_vNorm1) &&
	Clip(-in_vDirection.x, +in_vOrigin.x - in_vOBBHalfSize.x, in_mOBBOrient.forward(), io_rfT0, io_rfT1, out_vNorm0, out_vNorm1) &&
	Clip(+in_vDirection.y, -in_vOrigin.y - in_vOBBHalfSize.y, -1 * in_mOBBOrient.left(), io_rfT0, io_rfT1, out_vNorm0, out_vNorm1) &&
	Clip(-in_vDirection.y, +in_vOrigin.y - in_vOBBHalfSize.y, in_mOBBOrient.left(), io_rfT0, io_rfT1, out_vNorm0, out_vNorm1) &&
	Clip(+in_vDirection.z, -in_vOrigin.z - in_vOBBHalfSize.z, -1 * in_mOBBOrient.up(), io_rfT0, io_rfT1, out_vNorm0, out_vNorm1) &&
	Clip(-in_vDirection.z, +in_vOrigin.z - in_vOBBHalfSize.z, in_mOBBOrient.up(), io_rfT0, io_rfT1, out_vNorm0, out_vNorm1);*/

	return bNotEntirelyClipped && (io_rfT0 != fSaveT0 || io_rfT1 != fSaveT1);
}

bool IsIntersectSegment(const Segment2Df& inSegment, float inT, const OBB2Df& inBox, int& out_iQuantity, float out_fT[2], Vector2Df out_vPoint[2], Vector2Df out_vNormal[2])
{
	float SegmLen = inSegment.GetLength();

	// convert segment to box coordinates
	Vector2Df kDiff = inSegment.GetOrigin() - inBox.GetOrigin();

	Vector2Df kOrigin(DotProduct(kDiff, inBox.GetOrientation().forward), DotProduct(kDiff, inBox.GetOrientation().up));

	Vector2Df vSegDirectional = inSegment.GetDirection() * SegmLen;

	Vector2Df kDirection(DotProduct(vSegDirectional, inBox.GetOrientation().forward), DotProduct(vSegDirectional, inBox.GetOrientation().up));

	float fT0 = 0.0f;
	float fT1 = inT;
	Vector2Df vNorm0(0);
	Vector2Df vNorm1(0);
	bool bIntersects = IsIntersectSegment(kOrigin, kDirection, inBox.GetHalfSizes(), inBox.GetOrientation(), fT0, fT1, vNorm0, vNorm1);

	if (bIntersects)
	{
		if (fT0 > 0.0f)
		{
			//assert_andrey(!vNorm0 > 0);
			out_fT[0] = fT0;
			out_vPoint[0] = inSegment.GetOrigin() + fT0 * vSegDirectional;
			out_vNormal[0] = vNorm0;
			if (fT1 < inT)
			{
				//assert_andrey(!vNorm1 > 0);
				out_iQuantity = 2;
				out_fT[1] = fT1;
				out_vPoint[1] = inSegment.GetOrigin() + fT1 * vSegDirectional;
				out_vNormal[1] = vNorm1;
			}
			else
				out_iQuantity = 1;
		}
		else  // fT0 == 0
		{
			if (fT1 < inT)
			{
				//assert_andrey(!vNorm1 > 0);
				out_iQuantity = 1;
				out_fT[0] = fT1;
				out_vPoint[0] = inSegment.GetOrigin() + fT1 * vSegDirectional;
				out_vNormal[0] = vNorm1;
			}
			else  // fT1 == 1
			{
				// segment entirely in box
				out_iQuantity = 0;
			}
		}
	}
	else
	{
		out_iQuantity = 0;
	}

	return bIntersects;
}

bool IsIntersectSegment(const Segment2Df& in_Segment, float inT, const Circle2Df& inCircle, int& out_iQuantity, float out_fT[2], Vector2D<float> out_vPoint[2], Vector2D<float> out_vNormal[2])
{
	float InvRadius = 1 / inCircle.GetRadius();
	// set up quadratic Q(t) = a*t^2 + 2*b*t + c
	Vector2D<float> kDiff = in_Segment.GetOrigin() - inCircle.GetCenter();
	float fA = in_Segment.GetLength() * in_Segment.GetLength();
	float fInvA = in_Segment.GetInvLength() * in_Segment.GetInvLength();

	Vector2D<float> vSegDirectional = in_Segment.GetFullLengthDirection();
	float fB = DotProduct(kDiff, vSegDirectional);

	float fC = SqrLen(kDiff) - inCircle.GetRadius() * inCircle.GetRadius();

	// no intersection if Q(t) has no Real roots
	float afT[2];
	float fDiscr = fB*fB - fA*fC;

	if (abs(fDiscr) <= FLT_EPSILON)
		fDiscr = 0;

	if (fDiscr < (float)0.0)
	{
		out_iQuantity = 0;
		return false;
	}
	else if (fDiscr > 0.0)
	{
		float fRoot = sqrt(fDiscr);

		afT[0] = (-fB - fRoot)*fInvA;
		afT[1] = (-fB + fRoot)*fInvA;

		// assert: t0 < t1 since A > 0

		if (afT[0] > inT || afT[1] < 0.0)
		{
			out_iQuantity = 0;
			return false;
		}
		else
		{
			out_iQuantity = 2;
			out_fT[0] = max(afT[0], float(0));
			out_vPoint[0] = in_Segment.GetOrigin() + out_fT[0] * vSegDirectional;
			out_vNormal[0] = (out_vPoint[0] - inCircle.GetCenter()) * InvRadius;

			out_fT[1] = min(afT[1], inT);
			out_vPoint[1] = in_Segment.GetOrigin() + out_fT[1] * vSegDirectional;
			out_vNormal[1] = (out_vPoint[1] - inCircle.GetCenter()) * InvRadius;
			return true;
		}
	}
	else
	{
		afT[0] = -fB * fInvA;
		if (0.0f <= afT[0] && afT[0] <= inT)
		{
			out_iQuantity = 1;

			out_fT[0] = afT[0];
			out_vPoint[0] = in_Segment.GetOrigin() + out_fT[0] * vSegDirectional;
			out_vNormal[0] = (out_vPoint[0] - inCircle.GetCenter()) * InvRadius;

			return true;
		}
		else
		{
			out_iQuantity = 0;
			return false;
		}
	}
}