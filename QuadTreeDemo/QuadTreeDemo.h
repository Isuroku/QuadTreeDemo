
// QuadTreeDemo.h : main header file for the QuadTreeDemo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CQuadTreeDemoApp:
// See QuadTreeDemo.cpp for the implementation of this class
//

class CQuadTreeDemoApp : public CWinApp
{
public:
	CQuadTreeDemoApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CQuadTreeDemoApp theApp;
