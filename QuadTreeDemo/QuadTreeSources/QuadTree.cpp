#include "stdafx.h"
#include "QuadTree.h"
#include "PassInTree.h"
#include "IntersectPassObject.h"


CQuadTree::CQuadTree(IQuadTreeOwner& inOwner, float inMapSize, float inCellSize) :_owner(inOwner)
{
	_fCellSize = inCellSize;
	unsigned int dwMaxWide = GetMaxWide(); //max count of ushort
	//so max deep is 15 (MAX_TREE_DEPTH)
	// how many float units are in one unsigned short unit
	//full width of the map is ushort.MaxValue+1
	_fMetersPerWORD = inMapSize / (float)dwMaxWide; 
	_fWORDperMeter = 1.0f / _fMetersPerWORD;
}


CQuadTree::~CQuadTree()
{
}

bool CQuadTree::AddObject(IQuadTreeObject& inObject)
{
	Rect2Dus bound = GetAlignBound(inObject);
	SPassInTree PassParam(MAX_TREE_DEPTH, &inObject, bound, 0, GetMaxDepth());
	AddObstacleToLeaf(PassParam);
	return true;
}

bool CQuadTree::DeleteObject(IQuadTreeObject& inObject)
{
	Rect2Dus bound = GetAlignBound(inObject);
	SPassInTree PassParam(MAX_TREE_DEPTH, &inObject, bound, 0, GetMaxDepth());
	DeleteObstacleFromLeaf(PassParam);
	return true;
}

Rect2Dus CQuadTree::GetAlignBound(IQuadTreeObject& inObject)
{
	Rect2Df objrct = inObject.GetBoundBox();

	float fMax = GetSize() - GetMinCellSize() / 2;
	objrct.Bound(0, 0, fMax, fMax);

	Rect2Dus rct(static_cast<unsigned short>(objrct.GetLeft() * _fWORDperMeter), 
					static_cast<unsigned short>(objrct.GetTop() * _fWORDperMeter),
					static_cast<unsigned short>(objrct.GetRight() * _fWORDperMeter),
					static_cast<unsigned short>(objrct.GetBottom() * _fWORDperMeter));
	return rct;
}

void CQuadTree::OnLeafChanged(const CQuadLeaf& inLeaf, ELeafChangeType inType)
{
	_owner.OnLeafChangedHandler(&inLeaf, inType);
}


bool CQuadTree::IsValidCoord(Vector2Df inPoint) const
{
	if (inPoint.x < 0 || inPoint.y < 0)
		return false;

	if (inPoint.x >= GetSize() || inPoint.y >= GetSize())
		return false;

	return true;
}

IQuadTreeObject* CQuadTree::GetIntersectSegment(const Segment2Df& inSegment, float& ioT) const
{
	IQuadTreeObject* res = nullptr;
	vector<IQuadTreeObject*> checkedobjects;
	ForEachLeafBySegment(inSegment, ioT, [&checkedobjects, &inSegment, &ioT, &res](const CQuadLeaf& pleaf)
	{
		vector<IQuadTreeObject*> objects;
		pleaf.GetObjects(objects);
		//clear vector from already checked objects
		remove_if(begin(objects), end(objects), [&checkedobjects](IQuadTreeObject* obj)
		{
			return !AddSort(checkedobjects, obj);
		});

		res = GetIntersect(objects, inSegment, ioT);
		return res != nullptr;
	});

	return res;
}

void CQuadTree::ForEachLeafBySegment(const Segment2Df& inSegment, float& ioT, std::function<bool(const CQuadLeaf&)> const&  pred) const
{
	if (!IsValidCoord(inSegment.GetOrigin()))
		return;

	if (!IsValidCoord(inSegment.GetDestination()))
		return;

	float fWORDperMeter = GetWORDperMeter();
	SIntersectPassObject Pass(inSegment.GetOrigin(), inSegment.GetDestination(), fWORDperMeter);

	CQuadLeaf* pleaf = GetDeepestLeaf(Pass.Start);
	if (pleaf == nullptr)
	{
		_owner.ErrorLog(L"GetDeepestLeaf couldn't find a leaf!");
		return;
	}

	if (Pass.Start == Pass.End)
	{
		pred(*pleaf);
		return;
	}

	while (Pass.T < ioT && pleaf != nullptr)
	{
		if (pred(*pleaf))
			return;

		Rect2Dui rctLB = pleaf->GetBoundRect();

		unsigned short uiShiftX = 0;
		double dblShiftX = 2; //more than 1
		if (!Pass.InfinityX)
		{
			if (Pass.ForwardX)
				uiShiftX = (unsigned short)(rctLB.GetRight() - Pass.Current.x);
			else
				uiShiftX = (unsigned short)(Pass.Current.x - rctLB.GetLeft());

			dblShiftX = uiShiftX * Pass.Kx;
		}

		unsigned short uiShiftY = 0;
		double dblShiftY = 2;
		if (!Pass.InfinityY)
		{
			if (Pass.ForwardY)
				uiShiftY = (unsigned short)(rctLB.GetBottom() - Pass.Current.y);
			else
				uiShiftY = (unsigned short)(Pass.Current.y - rctLB.GetTop());

			dblShiftY = uiShiftY * Pass.Ky;
		}

		EQTSide Side;
		unsigned short uiCoord = 0;
		if (dblShiftX < dblShiftY)
		{
			if (dblShiftX > ioT - Pass.T)
				return;

			if (Pass.ForwardX)
			{
				Side = EQTSide::Right;
				Pass.Current.x += uiShiftX;
			}
			else
			{
				Side = EQTSide::Left;
				Pass.Current.x -= uiShiftX;
			}

			if (!Pass.InfinityY)
			{
				double deltaY = dblShiftX / Pass.Ky + 0.5; //for cutting round

				if (Pass.ForwardY)
					Pass.Current.y += (unsigned short)deltaY;
				else
					Pass.Current.y -= (unsigned short)deltaY;
			}

			Pass.T += dblShiftX;
			uiCoord = Pass.Current.y;
		}
		else
		{
			if (dblShiftY > ioT - Pass.T)
				return;

			if (Pass.ForwardY)
			{
				Side = EQTSide::Bottom;
				Pass.Current.y += uiShiftY;
			}
			else
			{
				Side = EQTSide::Top;
				Pass.Current.y -= uiShiftY;
			}

			if (!Pass.InfinityX)
			{
				double deltaX = dblShiftY / Pass.Kx + 0.5;
				if (Pass.ForwardX)
					Pass.Current.x += (unsigned short)deltaX;
				else
					Pass.Current.x -= (unsigned short)deltaX;
			}

			Pass.T += dblShiftY;
			uiCoord = Pass.Current.x;
		}

		pleaf = pleaf->GetNeighbourLeaf(Side, uiCoord);
	}
}