
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "QuadTreeDemo.h"
#include "ChildView.h"
#include "MainFrm.h"
#include "Obstacle.h"
#include "Math/OBB2D.h"
#include "QuadTreeSources\QuadTree.h"
#include <algorithm>
#include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView():
	_intersecT(1)
{
	_owner = nullptr;
	_LMBState = ELMBState::Select;

	//_crtBreakAlloc = 534;
}

CChildView::~CChildView()
{
	_obstacles.clear();
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


void CChildView::DebugLog(std::wstring inText)
{
	if(_owner != nullptr)
		_owner->DebugLog(inText);
}

void CChildView::ErrorLog(std::wstring inText)
{
	if (_owner != nullptr)
		_owner->ErrorLog(inText);
}


// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType)
{
	__super::CalcWindowRect(lpClientRect, nAdjustType);

	if (_tree == nullptr)
	{
		float mapwidth = static_cast<float>(min(lpClientRect->right - lpClientRect->left, lpClientRect->bottom - lpClientRect->top));
		_tree = make_unique<CQuadTree>(*this, mapwidth, 20.0f);
	}
}

void CChildView::DrawTree(CPaintDC& inDC, bool inUseRepaintRect)
{
	if (_tree == nullptr)
		return;

	CPen leafPen(PS_SOLID, 1, RGB(0, 0, 255));

	CBrush clearbrush;
	clearbrush.CreateSolidBrush(RGB(255, 255, 255));

	CBrush closedbrush;
	closedbrush.CreateSolidBrush(RGB(155, 155, 155));

	_tree->ForEachLeaf([&inDC, &clearbrush, &closedbrush, &leafPen](const CQuadLeaf& inLeaf)
	{
		inDC.SelectObject(&leafPen);
		if (inLeaf.IsClear())
			inDC.SelectObject(&clearbrush);
		else
			inDC.SelectObject(&closedbrush);

		Rect2Df boundrect = inLeaf.GetBound();

		inDC.Rectangle(&boundrect.GetWinRect());
		/*if(inLeaf.IsClear())
		inDC.FrameRect(&boundrect.GetWinRect(), &brush);
		else
		inDC.FillRect(&boundrect.GetWinRect(), &brush);*/
	});
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CPen* oldpen = dc.GetCurrentPen();
	CBrush* oldbrush = dc.GetCurrentBrush();

	DrawTree(dc, false);

	//draw intersect test segment
	if (_intersectsegment.IsDefine())
	{
		CPen intersectPen(PS_SOLID, 1, RGB(255, 0, 0));
		CPen noneintersectPen(PS_SOLID, 1, RGB(0, 55, 55));

		dc.MoveTo(static_cast<int>(_intersectsegment.GetOrigin().x), static_cast<int>(_intersectsegment.GetOrigin().y));
		if (_intersecT != 0.0f && _intersecT < 1.0f)
		{
			dc.SelectObject(&intersectPen);
			Vector2Df intersectpoint = _intersectsegment.GetOrigin() + _intersectsegment.GetFullLengthDirection() * _intersecT;
			Vector2Di ip(static_cast<int>(intersectpoint.x), static_cast<int>(intersectpoint.y));
			dc.LineTo(ip.x, ip.y);
			int w = 3;
			dc.Ellipse(ip.x - w, ip.y - w, ip.x + w, ip.y + w);
		}

		dc.SelectObject(&noneintersectPen);
		Vector2Df endpoint = _intersectsegment.GetDestination();
		dc.LineTo(static_cast<int>(endpoint.x), static_cast<int>(endpoint.y));
	}

	//draw all obstacles
	CPen shapePen(PS_SOLID, 1, RGB(255, 0, 0));
	dc.SelectObject(&shapePen);

	CBrush brush;
	brush.CreateStockObject(NULL_BRUSH);
	dc.SelectObject(&brush);
	
	for_each(_obstacles.begin(), _obstacles.end(), [&dc](shared_ptr<CObstacle> o)	{ o->Draw(dc); });
	

	dc.SelectObject(oldpen);
	dc.SelectObject(oldbrush);
	// Do not call CWnd::OnPaint() for painting messages
}



void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point)
{
	switch (_LMBState)
	{
	case ELMBState::AddCircleObstacle:
	case ELMBState::AddOBBObstacle:
		AddObstacle(_LMBState, point);
		break;
	case ELMBState::IntersectSegment:
		{
			_intersectsegment.ChangeOrigin(Vector2Df(point));
			CheckIntersect();
			Invalidate();
			break;
		}
	}
	
	__super::OnLButtonUp(nFlags, point);
}

void CChildView::AddObstacle(ELMBState inState, const CPoint& point)
{
	if (inState != ELMBState::AddCircleObstacle && 
		inState != ELMBState::AddOBBObstacle)
		return;

	CObstacle* pobst = nullptr;
	switch (inState)
	{
		case ELMBState::AddCircleObstacle: pobst = new CObstacleCircle(point, 50); break;
		case ELMBState::AddOBBObstacle: pobst = new CObstacleOBB(point, 50, 150); break;
	}

	if (pobst != nullptr)
	{
		_tree->AddObject(*pobst);
		_obstacles.push_back(shared_ptr<CObstacle>(pobst));
		Invalidate();
		DebugLog(pobst->ToString(-1));
	}
}

void CChildView::RemoveObstacle(const CPoint& point)
{
	Vector2Df pnt(point);

	auto delit = remove_if(_obstacles.begin(), _obstacles.end(), [&pnt, this](shared_ptr<CObstacle>& obst)
	{
		if (obst->IsPointInside(pnt))
		{
			_tree->DeleteObject(*obst.get());
			return true;
		}
		return false;
	});

	if (delit != _obstacles.end())
	{
		_obstacles.erase(delit, _obstacles.end());
		Invalidate();
	}
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point)
{
	switch (_LMBState)
	{
	case ELMBState::AddCircleObstacle:
	case ELMBState::AddOBBObstacle:
		RemoveObstacle(point);
		break;
	case ELMBState::IntersectSegment:
		{
			_intersectsegment.ChangeDestination(Vector2Df(point));
			CheckIntersect();
			Invalidate();
			break;
		}
	}

	__super::OnRButtonUp(nFlags, point);
}


void CChildView::OnLeafChangedHandler(const CQuadLeaf* inLeaf, ELeafChangeType inType)
{
	if (inLeaf == nullptr)
		return;

	_repaintrect.push_back(inLeaf->GetBound());
}


void CChildView::SetTestObstacles()
{
	AddObstacle(ELMBState::AddCircleObstacle, CPoint(236, 253));
	/*AddObstacle(ELMBState::AddCircleObstacle, CPoint(190, 109));
	AddObstacle(ELMBState::AddCircleObstacle, CPoint(273, 189));
	AddObstacle(ELMBState::AddCircleObstacle, CPoint(310, 252));*/
}

void CChildView::CheckIntersect()
{
	if (_tree == nullptr)
		return;

	float dT = 1;
	IQuadTreeObject* pObj = _tree->GetIntersectSegment(_intersectsegment, dT);

	/*vector<IQuadTreeObject*> objects;
	for_each(begin(_obstacles), end(_obstacles), [&objects](shared_ptr<CObstacle>& pObst) { objects.push_back(pObst.get());	});

	IQuadTreeObject* pObj = GetIntersect(objects, _intersectsegment, dT);*/

	_intersecT = static_cast<float>(dT);

	if (pObj == nullptr)
	{
		DebugLog(L"Intersect wasn't found.");
		return;
	}

	DebugLog(L"Intersect: object " + pObj->ToString(-1) + L", T = " + to_wstring(_intersecT));
}
