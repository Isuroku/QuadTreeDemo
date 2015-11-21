#pragma once
#include "afxwin.h"
#include <string>

using namespace std;

// CLogViewer dialog

class CLogViewer : public CDialog
{
	DECLARE_DYNAMIC(CLogViewer)

public:
	CLogViewer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogViewer();

	void AddLog(wstring inText);
	void PrintHelpText();

// Dialog Data
	enum { IDD = IDD_LOGVIEWER };

protected:
	bool _init = false;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	CEdit _logedit;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnChangeLogedit();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};
