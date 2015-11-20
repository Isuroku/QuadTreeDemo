#pragma once
#include <stack>
#include <vector>
#include "QuadLeaf.h"
#include "IQuadInterfaces.h"
#include "PassInTree.h"

class CQuadTree : public CQuadLeaf
{
public:
	CQuadTree(IQuadTreeOwner& inOwner, float inMapSize, float inCellSize);
	virtual ~CQuadTree();

	bool					AddObject(IQuadTreeObject& inObject);
	bool					DeleteObject(IQuadTreeObject& inObject);
	virtual float			GetMetersPerWORD() const override { return _fMetersPerWORD; }
	float					GetWORDperMeter() const { return 1.0f / _fMetersPerWORD; }

	virtual IQuadTreeOwner* GetOwner() const override { return &_owner; }

	IQuadTreeObject*		GetIntersectSegment(const Segment2Df& inSegment, float& ioT) const;
	void					ForEachLeafBySegment(const Segment2Df& inSegment, float& ioT, std::function<bool(const CQuadLeaf&)> const&  pred) const;

	inline float			GetMinCellSize() { return _fCellSize; }
	
	inline float GetSize() const
	{
		unsigned int dwMaxWide = GetMaxWide();
		return _fMetersPerWORD * dwMaxWide;
	}

	inline byte GetMaxDepth()
	{
		float fR = _fWORDperMeter * GetMinCellSize();
		return GetDepth((unsigned short)fR);
	}

	inline byte				GetDepth(unsigned short inSize) { return static_cast<byte>(16 - GetHighestSetBit(inSize)); }

protected:
	virtual void			OnLeafChanged(const CQuadLeaf& inLeaf, ELeafChangeType inType) override;

private:

	IQuadTreeOwner&			_owner;

	float					_fMetersPerWORD;
	float					_fWORDperMeter;
	float					_fCellSize;

	stack<CQuadLeaf*>		_storedleaves;

	Rect2Dus				GetAlignBound(IQuadTreeObject& inObject);
	bool					IsValidCoord(Vector2Df inPoint) const;
};

