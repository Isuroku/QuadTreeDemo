// LogViewer.cpp : implementation file
//

#include "stdafx.h"
#include "QuadTreeDemo.h"
#include "LogViewer.h"
#include "afxdialogex.h"


// CLogViewer dialog

IMPLEMENT_DYNAMIC(CLogViewer, CDialog)

CLogViewer::CLogViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CLogViewer::IDD, pParent)
{
	
}

CLogViewer::~CLogViewer()
{
}

void CLogViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGEDIT, _logedit);
}


BEGIN_MESSAGE_MAP(CLogViewer, CDialog)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_LOGEDIT, &CLogViewer::OnEnChangeLogedit)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CLogViewer message handlers

void CLogViewer::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}


void CLogViewer::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (IsWindowVisible())
		GetDlgItem(IDC_LOGEDIT)->SetWindowPos(nullptr, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOMOVE | SWP_SHOWWINDOW);
}


void CLogViewer::OnEnChangeLogedit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CLogViewer::AddLog(wstring inText)
{
	static int iLine = 0;

	CString oldstr;
	_logedit.GetWindowTextW(oldstr);
	if (oldstr.GetLength() > 0)
		oldstr += L"\r\n";

	CString frm;
	frm.Format(L"%d %s", iLine++, inText.c_str());
	oldstr += frm;

	_logedit.SetWindowTextW(oldstr);

	_logedit.LineScroll(_logedit.GetLineCount());
}


void CLogViewer::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	//AddLog(L"Test");
}
