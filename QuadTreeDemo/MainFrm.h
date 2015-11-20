
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "ChildView.h"

class CLogViewer;

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void DebugLog(std::wstring inText);
	void ErrorLog(std::wstring inText);

private:  // control bar embedded members
	CToolBar				m_wndToolBar;
	CStatusBar				m_wndStatusBar;
	CChildView				m_wndView;
	unique_ptr<CLogViewer>	_logviewer;

	void					CreateLogViewer();

// Generated message map functions
private:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnLMBSelectCmd();
	afx_msg void OnUpdateLMBSelectCmd(CCmdUI* pCmdUI);
	afx_msg void OnLMBCircleCmd();
	afx_msg void OnUpdateLMBCircleCmd(CCmdUI* pCmdUI);
	afx_msg void OnLMBRectCmd();
	afx_msg void OnUpdateLMBRectCmd(CCmdUI* pCmdUI);
	afx_msg void OnShowLogsCmd();
	afx_msg void OnUpdateShowLogsCmd(CCmdUI* pCmdUI);
	afx_msg void OnTestObstaclesCmd();
	afx_msg void OnIntersectStateCmd();
	afx_msg void OnUpdateIntersectStateCmd(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

};


