
// Kol2012.h : main header file for the Kol2012 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol2012App:
// See Kol2012.cpp for the implementation of this class
//

class CKol2012App : public CWinApp
{
public:
	CKol2012App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol2012App theApp;
