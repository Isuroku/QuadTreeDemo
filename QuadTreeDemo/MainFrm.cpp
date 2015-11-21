
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "QuadTreeDemo.h"

#include "MainFrm.h"
#include "LogViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_EDIT_LMBSELECT, &CMainFrame::OnLMBSelectCmd)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LMBSELECT, &CMainFrame::OnUpdateLMBSelectCmd)
	ON_COMMAND(ID_EDIT_LMBCIRCLE, &CMainFrame::OnLMBCircleCmd)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LMBCIRCLE, &CMainFrame::OnUpdateLMBCircleCmd)
	ON_COMMAND(ID_EDIT_LMBRECT, &CMainFrame::OnLMBRectCmd)
	ON_UPDATE_COMMAND_UI(ID_EDIT_LMBRECT, &CMainFrame::OnUpdateLMBRectCmd)
	ON_COMMAND(ID_SHOWLOGS, &CMainFrame::OnShowLogsCmd)
	ON_UPDATE_COMMAND_UI(ID_SHOWLOGS, &CMainFrame::OnUpdateShowLogsCmd)
	ON_COMMAND(ID_TESTOBST, &CMainFrame::OnTestObstaclesCmd)
	ON_COMMAND(ID_CLEARALLBTN, &CMainFrame::OnClearAllObstaclesCmd)
	ON_COMMAND(ID_INTERSECTBTN, &CMainFrame::OnIntersectStateCmd)
	ON_UPDATE_COMMAND_UI(ID_INTERSECTBTN, &CMainFrame::OnUpdateIntersectStateCmd)
	ON_COMMAND(ID_APP_ABOUT, &OnHelpBtnCmd)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	m_wndView.SetOwner(this);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	CreateLogViewer();

	return 0;
}

const int MainWindowWidth = 713;
const int MainWindowHeight = 800;
const int LogWindowWidth = 400;

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.cx = MainWindowWidth;
	cs.cy = MainWindowHeight;
	cs.style &= ~WS_MAXIMIZEBOX;
	cs.style &= ~WS_SIZEBOX;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

void CMainFrame::CreateLogViewer()
{
	if (!_logviewer)
	{
		_logviewer = make_unique<CLogViewer>(this);
		_logviewer->Create(CLogViewer::IDD);
	}
	_logviewer->ShowWindow(SW_SHOW);

	WINDOWINFO info;
	GetWindowInfo(&info);

	_logviewer->SetWindowPos(this, info.rcWindow.right, info.rcWindow.top, LogWindowWidth, MainWindowHeight, 0);
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnLMBSelectCmd()
{
	m_wndView.SetLMBState(ELMBState::Select);
}

void CMainFrame::OnUpdateLMBSelectCmd(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndView.GetLMBState() == ELMBState::Select);
}

void CMainFrame::OnLMBCircleCmd()
{
	m_wndView.SetLMBState(ELMBState::AddCircleObstacle);
}

void CMainFrame::DebugLog(std::wstring inText)
{
	_logviewer->AddLog(inText);
}

void CMainFrame::ErrorLog(std::wstring inText)
{
	_logviewer->AddLog(L"Error!!!: " + inText);
}

void CMainFrame::OnUpdateLMBCircleCmd(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndView.GetLMBState() == ELMBState::AddCircleObstacle);
}

void CMainFrame::OnLMBRectCmd()
{
	m_wndView.SetLMBState(ELMBState::AddOBBObstacle);
}

void CMainFrame::OnUpdateLMBRectCmd(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndView.GetLMBState() == ELMBState::AddOBBObstacle);
}

void CMainFrame::OnIntersectStateCmd()
{
	m_wndView.SetLMBState(ELMBState::IntersectSegment);
}

void CMainFrame::OnUpdateIntersectStateCmd(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndView.GetLMBState() == ELMBState::IntersectSegment);
}

void CMainFrame::OnShowLogsCmd()
{
	CreateLogViewer();
}

void CMainFrame::OnTestObstaclesCmd()
{
	m_wndView.SetTestObstacles();
}

void CMainFrame::OnClearAllObstaclesCmd()
{
	m_wndView.ClearAllObstacles();
}

void CMainFrame::OnUpdateShowLogsCmd(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(_logviewer->IsWindowVisible());
}

void CMainFrame::OnHelpBtnCmd()
{
	_logviewer->PrintHelpText();
}