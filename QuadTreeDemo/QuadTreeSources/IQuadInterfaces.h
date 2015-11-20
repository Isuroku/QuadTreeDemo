#pragma once
#include <string>
#include <vector>
#include "../Math/Rect2D.h"
#include "../Math/Segment2D.h"

enum class ELeafChangeType { Create, Destroy, Update };

class CQuadLeaf;

class IQuadTreeOwner
{
public:
	virtual void DebugLog(std::wstring inText) = 0;
	virtual void ErrorLog(std::wstring inText) = 0;

	virtual void OnLeafChangedHandler(const CQuadLeaf* inLeaf, ELeafChangeType inType) = 0;
};

class IQuadTreeObject
{
public:
	virtual Rect2Df GetBoundBox() = 0;
	virtual bool TestIntersect(const Rect2Df& inBoundBox) = 0;
	virtual bool IsFullCovering(const Rect2Df& inBoundBox) = 0;
	virtual bool IsPointInside(Vector2Df inPoint) = 0;
	virtual wstring ToString(int inCountAfterPoint = MAXINT32) = 0;
	virtual bool IsIntersectSegment(const Segment2Df& inSegment, float& ioT) const = 0;
};

//class IQuadLeafProcessor
//{
//public:
//	virtual void Process(CQuadLeaf* inParent) = 0;
//};

inline IQuadTreeObject* GetIntersect(const std::vector<IQuadTreeObject*>& inObjects, const Segment2Df& inSegment, float& ioT)
{
	IQuadTreeObject* res = nullptr;
	for each (IQuadTreeObject* obj in inObjects)
	{
		if (obj != nullptr && obj->IsIntersectSegment(inSegment, ioT))
		{
			res = obj;
		}
	}
	return res;
}
