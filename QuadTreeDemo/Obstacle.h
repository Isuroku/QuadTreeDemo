#pragma once
#include "Math\Circle2D.h"
#include "Math\OBB2D.h"
#include "QuadTreeSources\IQuadInterfaces.h"

class CObstacle : public IQuadTreeObject
{
public:
	CObstacle();
	virtual ~CObstacle();

	virtual void Draw(CPaintDC& dc) = 0;
};

class CObstacleCircle : public CObstacle
{
public:
	CObstacleCircle(CPoint center, float radius);
	virtual ~CObstacleCircle();

	virtual Rect2Df GetBoundBox() override { return _circle.GetBoundBox(); }

	virtual void Draw(CPaintDC& dc) override;
	virtual wstring ToString(int inCountAfterPoint = MAXINT32) override;

	virtual bool TestIntersect(const Rect2Df& inBoundBox) override;
	virtual bool IsFullCovering(const Rect2Df& inBoundBox) override;
	virtual bool IsPointInside(Vector2Df inPoint)  override;

	virtual bool IsIntersectSegment(const Segment2Df& inSegment, float& ioT) const override;

private:
	Circle2Df _circle;
};

class CObstacleOBB : public CObstacle
{
public:
	CObstacleOBB(CPoint center, float inWidth, float inHeight);
	virtual ~CObstacleOBB();

	virtual Rect2Df GetBoundBox() override { return _obb.GetBoundBox(); }

	virtual void Draw(CPaintDC& dc) override;
	virtual wstring ToString(int inCountAfterPoint = MAXINT32) override;

	virtual bool TestIntersect(const Rect2Df& inBoundBox) override;
	virtual bool IsFullCovering(const Rect2Df& inBoundBox) override;
	virtual bool IsPointInside(Vector2Df inPoint)  override;

	virtual bool IsIntersectSegment(const Segment2Df& inSegment, float& ioT) const override;

	void Rotate(float inAngle)
	{
		_obb.Rotate(inAngle);
	}

private:
	OBB2Df _obb;
};

