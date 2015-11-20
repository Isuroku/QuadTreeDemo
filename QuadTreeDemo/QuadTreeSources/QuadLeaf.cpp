#include "stdafx.h"
#include "QuadLeaf.h"


CQuadLeaf::CQuadLeaf() : _parent(nullptr), _depth(0), _min(0, 0)
{
	_childs.fill(nullptr);
}

CQuadLeaf::CQuadLeaf(CQuadLeaf* inParent, const Vector2Dus inMin, byte inDepth) : 
	_parent(inParent), _depth(inDepth), _min(inMin)
{
	_childs.fill(nullptr);
}


CQuadLeaf::~CQuadLeaf()
{
	_objects.clear();
	DeleteChilds();
}

void CQuadLeaf::ForEachLeaf(std::function<void(const CQuadLeaf&)> const&  pred)
{
	if (IsChildPresent())
	{
		for (const auto& l : _childs)
			l->ForEachLeaf(pred);
	}
	else
		pred(*this);
}

Rect2Df CQuadLeaf::GetBound() const
{
	Rect2Dui uiR = GetBoundRect();
	float fM_In_WORD = GetMetersPerWORD();

	Rect2Df fR(uiR.GetLeft() * fM_In_WORD, uiR.GetTop() * fM_In_WORD, uiR.GetRight() * fM_In_WORD, uiR.GetBottom() * fM_In_WORD);
	return fR;
}

void CQuadLeaf::CreateChild()
{
	if (IsChildPresent())
		return;

	byte btChildDepth = _depth;
	btChildDepth++;

	unsigned int dwMaxWide = GetMaxWide();
	unsigned short wWide = static_cast<unsigned short>(dwMaxWide >> btChildDepth);

	Vector2Dus vMin(_min);
	_childs[0] = new CQuadLeaf(this, vMin, btChildDepth);
	vMin.ShiftX(wWide);
	_childs[1] = new CQuadLeaf(this, vMin, btChildDepth);
	vMin.Shift(-wWide, wWide);
	_childs[2] = new CQuadLeaf(this, vMin, btChildDepth);
	vMin.ShiftX(wWide);
	_childs[3] = new CQuadLeaf(this, vMin, btChildDepth);

	OnLeafChanged(*this, ELeafChangeType::Update);

	OnLeafChanged(*_childs[0], ELeafChangeType::Create);
	OnLeafChanged(*_childs[1], ELeafChangeType::Create);
	OnLeafChanged(*_childs[2], ELeafChangeType::Create);
	OnLeafChanged(*_childs[3], ELeafChangeType::Create);
}

void CQuadLeaf::AddObstacleToLeaf(const SPassInTree& inPassInfo)
{
	if (!TestIntersect(inPassInfo))
		return;

	if (!IsChildPresent() && inPassInfo._pobject == nullptr)
		return;

	if (inPassInfo.IsOnDepth() || CheckFullCovering(inPassInfo))
	{
		AddObject(inPassInfo._pobject, true);
		return;
	}

	if (!IsChildPresent())
		CreateChild();

	SPassInTree newpass(inPassInfo);
	newpass.m_btDepth++;

	for_each(begin(_childs), end(_childs), [this, &newpass](CQuadLeaf* l) { l->AddObstacleToLeaf(newpass); });

	TryToUnionChilds();
}

void CQuadLeaf::DeleteObstacleFromLeaf(const SPassInTree& inPassInfo)
{
	if (!TestIntersect(inPassInfo))
		return;

	if (!RemoveObject(inPassInfo._pobject) && IsChildPresent())
	{
		for_each(begin(_childs), end(_childs), [this, &inPassInfo](CQuadLeaf* l) { l->DeleteObstacleFromLeaf(inPassInfo); });
		TryToUnionChilds();
	}

}

bool CQuadLeaf::CheckFullCovering(const SPassInTree& inPassInfo)
{
	return false;

	if (inPassInfo._pobject == nullptr)
		return CheckFullCovering(inPassInfo.m_Bound);

	return inPassInfo._pobject->IsFullCovering(GetBound());
}

bool CQuadLeaf::CheckFullCovering(const Rect2Dus& m_Bound)
{
	Rect2Dui leafrect = GetBoundRect();
	if(m_Bound.GetLeft() > leafrect.GetLeft())
		return false;
	if (m_Bound.GetTop() > leafrect.GetTop())
		return false;
	if (m_Bound.GetRight() < leafrect.GetRight())
		return false;
	if (m_Bound.GetBottom() < leafrect.GetBottom())
		return false;
	return true;
}

bool CQuadLeaf::TestIntersect(const SPassInTree& inPassInfo)
{
	Rect2Df map_rect = GetBound();

	return inPassInfo._pobject == nullptr ||
		inPassInfo._pobject->TestIntersect(map_rect);
}

void CQuadLeaf::TryToUnionChilds()
{
	if (!IsChildPresent())
		return;

	if (any_of(begin(_childs), end(_childs), [](CQuadLeaf* l) { return l->IsChildPresent(); }))
		return;

	if (_childs[0]->IsEqual(*_childs[1]) && 
		_childs[1]->IsEqual(*_childs[2]) &&
		_childs[2]->IsEqual(*_childs[3]))
	{
		AssignObjects(_childs[0]->_objects);
		ClearChild(false);
	}
}

void CQuadLeaf::DeleteChilds()
{
	for_each(begin(_childs), end(_childs), [this](CQuadLeaf* l)	{ delete(l); });
	_childs.fill(nullptr);
}

void CQuadLeaf::ClearChild(bool inOnDestroy)
{
	if (!IsChildPresent())
		return;

	if (!inOnDestroy)
		for_each(begin(_childs), end(_childs), [this](CQuadLeaf* l) { OnLeafChanged(*l, ELeafChangeType::Destroy); });

	DeleteChilds();

	if (!inOnDestroy)
		OnLeafChanged(*this, ELeafChangeType::Update);
}

void CQuadLeaf::OnLeafChanged(const CQuadLeaf& inLeaf, ELeafChangeType inType)
{
	if (GetParent() == nullptr)
		return;

	GetParent()->OnLeafChanged(inLeaf, inType);
}

bool CQuadLeaf::IsEqual(const CQuadLeaf& other) const
{
	if (_depth != other._depth)
		return false;

	if (IsObjectPresent() != other.IsObjectPresent())
		return false;

	if (!IsObjectPresent())
		return true;

	if (_objects.size() != other._objects.size())
		return false;

	//_objects always in sorted state
	for (size_t i = 0; i < other._objects.size(); ++i)
	{
		if (_objects[i] != other._objects[i])
			return false;
	}
	return true;
}

void CQuadLeaf::AssignObjects(vector<IQuadTreeObject*> inObjects)
{
	for_each(begin(inObjects), end(inObjects), [this](IQuadTreeObject* pobj)
	{
		AddObject(pobj, false);
	});
}

template <typename TYPE>
bool AddSort(std::vector<TYPE>& ioObjects, TYPE inObject)
{
	auto pos = lower_bound(begin(ioObjects), end(ioObjects), inObject);
	if (pos == end(ioObjects))
	{
		ioObjects.push_back(inObject);
		return true;
	}

	if (*pos == inObject)
		return false;

	ioObjects.insert(pos, inObject);
	return true;
}

void CQuadLeaf::AddObject(IQuadTreeObject* inObject, bool inEvent)
{
	bool wasAdded = AddSort(_objects, inObject);
	if (!wasAdded)
	{
		GetOwner()->ErrorLog(L"Attempt to add a duplicate objects.");
		return;
	}

	if(inEvent)
		OnLeafChanged(*this, ELeafChangeType::Update);
}

bool CQuadLeaf::RemoveObject(IQuadTreeObject* inObject)
{
	auto pos = lower_bound(begin(_objects), end(_objects), inObject);
	if (pos == end(_objects))
		return false;

	if (*pos != inObject)
		return false;

	_objects.erase(pos);

	OnLeafChanged(*this, ELeafChangeType::Update);

	return true;
}

CQuadLeaf* CQuadLeaf::GetDeepestLeaf(const Vector2Dus& inPoint) const
{
	return GetDeepestLeaf(inPoint, _depth);
}

CQuadLeaf* CQuadLeaf::GetDeepestLeaf(const Vector2Dus& inPoint, byte inDepth) const
{
	if (!IsChildPresent())
		return const_cast<CQuadLeaf*>(this);

	int btCrd1 = inPoint.x >> (MAX_TREE_DEPTH - inDepth) & 1;
	int btCrd2 = inPoint.y >> (MAX_TREE_DEPTH - inDepth - 1) & 2;

	byte btCrd = (byte)(btCrd1 + btCrd2);

	byte btDepth = inDepth;
	btDepth++;
	return _childs[btCrd]->GetDeepestLeaf(inPoint, btDepth);
}

void CQuadLeaf::GetObjects(std::vector<IQuadTreeObject*>& outObjects) const
{
	if (outObjects.size() == 0)
		outObjects.assign(begin(_objects), end(_objects));
	else
		for each(IQuadTreeObject* o in _objects)
			AddSort(outObjects, o);

	if (_parent != nullptr)
		_parent->GetObjects(outObjects);
}

byte CQuadLeaf::GetIndexInParent(CQuadLeaf* inChild) const
{
	if (inChild == nullptr)
		return 4;

	if (inChild == _childs[0])
		return 0;
	if (inChild == _childs[1])
		return 1;
	if (inChild == _childs[2])
		return 2;
	if (inChild == _childs[3])
		return 3;

	return 4;
}

CQuadLeaf* CQuadLeaf::GetNeighbourLeaf(EQTSide in_Side, unsigned short in_uiCoord)
{
	if (_parent == nullptr)
		return nullptr;

	byte uiInd = _parent->GetIndexInParent(this);
	if (uiInd > 3)
		return nullptr;

	//4 - there isn't sibling neigbough
	static byte NeaigbourLeafIndex[4][4] =
	{
		{ 4, 0, 4, 2 }, //l
		{ 4, 4, 0, 1 }, //t
		{ 1, 4, 3, 4 }, //r
		{ 2, 3, 4, 4 }, //b
	};

	byte uiNI = NeaigbourLeafIndex[(byte)in_Side][uiInd];
	if (uiNI == 4)
		return _parent->GetNeighbourLeaf(in_Side, in_uiCoord);

	return _parent->GetChild(uiNI)->GetWallLeaf(in_Side, in_uiCoord);
}

CQuadLeaf* CQuadLeaf::GetChild(byte inIndex)
{
	if (inIndex >= 4)
		throw std::invalid_argument("index error");
	return _childs[inIndex];
}

CQuadLeaf* CQuadLeaf::GetWallLeaf(EQTSide inSide, unsigned short inCoord)
{
	static byte pXY[4] = { 1, 0, 1, 0 }; //axis (abscissa and ordinate) the opposite side (LTRB)

	unsigned short uiMin = _min[pXY[(byte)inSide]];
	if (inCoord < uiMin)
		return nullptr;

	unsigned int uiMax = GetWidth() + uiMin;
	if (inCoord > uiMax)
		return nullptr;

	if (!IsChildPresent())
		return this;

	//indices of other cells leaning against your wall
	static byte SideIndices[4][2] =
	{
		{ 1, 3 }, //l
		{ 2, 3 }, //t
		{ 0, 2 }, //r
		{ 0, 1 }, //b
	};

	CQuadLeaf* pL = GetChild(SideIndices[(byte)inSide][0])->GetWallLeaf(inSide, inCoord);
	if (pL == nullptr)
		pL = GetChild(SideIndices[(byte)inSide][1])->GetWallLeaf(inSide, inCoord);

	return pL;
}