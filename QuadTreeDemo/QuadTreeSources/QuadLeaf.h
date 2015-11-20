#pragma once
#include <array>
#include <memory>
#include <vector>
#include "../Math/Rect2D.h"
#include "PassInTree.h"
#include <functional>

using namespace std;

class CQuadLeaf;
typedef void(*ForEachPred)(const CQuadLeaf&);

class CQuadLeaf
{
public:
	CQuadLeaf();
	CQuadLeaf(CQuadLeaf* inParent, const Vector2Dus inMin, byte inDepth);
	virtual ~CQuadLeaf();

	static unsigned int		GetMaxWide() { return 0x10000; }
	
	Rect2Df					GetBound() const;
	inline byte				GetDepth() const { return _depth; }
	inline bool				IsObjectPresent() const { return !_objects.empty(); }
	inline bool				IsClear() const { return !IsObjectPresent() && (_parent == nullptr || _parent->IsClear()); }

	inline unsigned int		GetWidth() const
	{
		unsigned int dwWide = GetMaxWide();
		dwWide = dwWide >> _depth;
		return dwWide;
	}

	inline Rect2Dui			GetBoundRect() const
	{
		unsigned int dwWide = GetWidth();
		Vector2Dui mincorner(_min.x, _min.y);
		Rect2Dui rct(mincorner, dwWide, dwWide);
		return rct;
	}

	virtual float			GetMetersPerWORD() const
	{
		if (_parent != nullptr)
			return _parent->GetMetersPerWORD();
		return 0;
	}

	bool								IsEqual(const CQuadLeaf& other) const;

	void								ForEachLeaf(std::function<void (const CQuadLeaf&)> const&  pred);

	void								GetObjects(std::vector<IQuadTreeObject*>& outObjects) const;

	CQuadLeaf*							GetNeighbourLeaf(EQTSide in_Side, unsigned short in_uiCoord);
	byte								GetIndexInParent(CQuadLeaf* inChild) const;

protected:
	const byte							MAX_TREE_DEPTH = 15;

	CQuadLeaf*							_parent;
	array<CQuadLeaf*, 4>				_childs;
	vector<IQuadTreeObject*>			_objects;

	byte								_depth;
	Vector2Dus							_min;

	CQuadLeaf*							GetParent() { return _parent; }
	CQuadLeaf*							GetChild(byte inIndex);

	bool								TestIntersect(const SPassInTree& inPassInfo);
	void								AddObstacleToLeaf(const SPassInTree& inPassInfo);
	void								DeleteObstacleFromLeaf(const SPassInTree& inPassInfo);
	void								CreateChild();
	void								TryToUnionChilds();
	void								ClearChild(bool inOnDestroy);
	void								AssignObjects(vector<IQuadTreeObject*> inObjects);
	void								AddObject(IQuadTreeObject* inObject, bool inEvent);
	bool								RemoveObject(IQuadTreeObject* inObject);
	bool								CheckFullCovering(const Rect2Dus& m_Bound);
	bool								CheckFullCovering(const SPassInTree& inPassInfo);
	void								DeleteChilds();

	CQuadLeaf*							GetDeepestLeaf(const Vector2Dus& inPoint) const;
	CQuadLeaf*							GetDeepestLeaf(const Vector2Dus& inPoint, byte inDepth) const;

	CQuadLeaf*							GetWallLeaf(EQTSide inSide, unsigned short inCoord);

	inline bool							IsChildPresent() const { return _childs[0] != nullptr; }

	virtual void						OnLeafChanged(const CQuadLeaf& inLeaf, ELeafChangeType inType);

	virtual IQuadTreeOwner*				GetOwner() const
	{
		if (_parent != nullptr)
			return _parent->GetOwner();
		return nullptr;
	}
};

template <typename TYPE>
bool AddSort(std::vector<TYPE>& ioObjects, TYPE inObject); //true - was added
