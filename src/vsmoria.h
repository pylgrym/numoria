
// vsmoria.h : main header file for the vsmoria application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CvsmoriaApp:
// See vsmoria.cpp for the implementation of this class
//

class CvsmoriaApp : public CWinApp
{
public:
	CvsmoriaApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CvsmoriaApp theApp;
