#include "stdafx.h"
#include "Obstacle.h"
#include "Math\Intersect.h"

//-------- base class ----
CObstacle::CObstacle() {}

CObstacle::~CObstacle() {}

//-------- circle ----
CObstacleCircle::CObstacleCircle(CPoint center, float radius) : _circle(center, radius){}

CObstacleCircle::~CObstacleCircle() {}

void CObstacleCircle::Draw(CPaintDC& dc)
{
	Rect2Df rct = _circle.GetBoundBox();
	dc.Ellipse(&rct.GetWinRect());
	//dc.AngleArc(_circle.GetCenter().x, _circle.GetCenter().y, _circle.GetRadius(), 0, 360);
}

wstring CObstacleCircle::ToString(int inCountAfterPoint)
{
	return L"Circle: " + _circle.ToString(inCountAfterPoint);
}

bool CObstacleCircle::TestIntersect(const Rect2Df& inBoundBox)
{
	return ::TestIntersectionBoxSphere(inBoundBox, _circle);
}

bool CObstacleCircle::IsFullCovering(const Rect2Df& inBoundBox)
{
	bool outside = false;
	for (int i = 0; i < 4 && !outside; i++)
		outside = !_circle.IsPointInside(inBoundBox.GetCorner(i));
	return !outside;
}

bool CObstacleCircle::IsPointInside(Vector2Df inPoint)
{
	return _circle.IsPointInside(inPoint);
}

bool CObstacleCircle::IsIntersectSegment(const Segment2Df& inSegment, float& ioT) const
{
	int iQuantity = 0;
	float fT[2];
	Vector2Df vPoints[2];
	Vector2Df vNormals[2];
	bool res = ::IsIntersectSegment(inSegment, ioT, _circle, iQuantity, fT, vPoints, vNormals);
	if (iQuantity > 0 && ioT > fT[0])
		ioT = fT[0];
	return res;
}

//-------- OBB ----

CObstacleOBB::CObstacleOBB(CPoint center, float inWidth, float inHeight) : _obb(center, inWidth, inHeight) {}

CObstacleOBB::~CObstacleOBB() {}

const int VertCount = 4;
void CObstacleOBB::Draw(CPaintDC& dc)
{
	//Rect2Df rct = _obb.GetBoundBox();
	//dc.Rectangle(&rct.GetWinRect());

	Vector2Df av[VertCount];
	_obb.GetVertices(av);

	POINT ap[VertCount];
	for (int i = 0; i < VertCount; i++)
	{
		ap[i].x = static_cast<LONG>(av[i].x);
		ap[i].y = static_cast<LONG>(av[i].y);
	}

	dc.Polygon(ap, VertCount);
}

bool CObstacleOBB::TestIntersect(const Rect2Df& inBoundBox)
{
	OBB2Df obb(inBoundBox.GetOrigin(), inBoundBox.GetWidth(), inBoundBox.GetHeight());
	return ::TestIntersectionBoxBox(obb, _obb);
}

bool CObstacleOBB::IsFullCovering(const Rect2Df& inBoundBox)
{
	bool outside = false;
	for (int i = 0; i < 4 && !outside; i++)
		outside = !_obb.IsPointInside(inBoundBox.GetCorner(i));
	return !outside;
}

bool CObstacleOBB::IsPointInside(Vector2Df inPoint)
{
	return _obb.IsPointInside(inPoint);
}

wstring CObstacleOBB::ToString(int inCountAfterPoint)
{
	return L"OBB: " + _obb.ToString(inCountAfterPoint);
}

bool CObstacleOBB::IsIntersectSegment(const Segment2Df& inSegment, float& ioT) const
{
	int iQuantity = 0;
	float fT[2];
	Vector2Df vPoints[2];
	Vector2Df vNormals[2];
	bool res = ::IsIntersectSegment(inSegment, ioT, _obb, iQuantity, fT, vPoints, vNormals);
	if (iQuantity > 0 && ioT > fT[0])
		ioT = fT[0];
	return res;
}
