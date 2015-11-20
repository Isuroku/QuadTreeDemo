#pragma once
#include <vector>
#include <memory>
#include "QuadTreeSources\IQuadInterfaces.h"

using namespace std;

enum class ELMBState { Select, AddCircleObstacle, AddOBBObstacle, IntersectSegment };

class CObstacle;
class CQuadTree;
class CQuadLeaf;

class CMainFrame;

[event_receiver(native)]
class CChildView : public CWnd, public IQuadTreeOwner
{
// Construction
public:
	CChildView();

	virtual void					DebugLog(std::wstring inText) override;
	virtual void					ErrorLog(std::wstring inText) override;
	virtual void					OnLeafChangedHandler(const CQuadLeaf* inLeaf, ELeafChangeType inType) override;

protected:
	virtual BOOL					PreCreateWindow(CREATESTRUCT& cs);

	CMainFrame*						_owner;

	ELMBState						_LMBState;

	vector<shared_ptr<CObstacle>>	_obstacles;
	vector<Rect2Df>					_repaintrect;

	unique_ptr<CQuadTree>			_tree;

	//segment for intersect test
	Segment2Df						_intersectsegment;
	float							_intersecT;

	void							DrawTree(CPaintDC& inDC, bool inUseRepaintRect);
	void							AddObstacle(ELMBState inState, const CPoint& point);
	void							RemoveObstacle(const CPoint& point);
	void							CheckIntersect();
	
// Implementation
public:
	virtual							~CChildView();
	ELMBState						GetLMBState() { return _LMBState; }
	void							SetLMBState(ELMBState inState) { _LMBState = inState; }
	void							SetOwner(CMainFrame* inOwner) { _owner = inOwner; }
	void							SetTestObstacles();

	

	// Generated message map functions
protected:
	afx_msg void					OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void					OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual void					CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	afx_msg void					OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void					OnRButtonUp(UINT nFlags, CPoint point);
};

