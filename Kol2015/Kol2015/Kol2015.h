
// Kol2015.h : main header file for the Kol2015 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol2015App:
// See Kol2015.cpp for the implementation of this class
//

class CKol2015App : public CWinApp
{
public:
	CKol2015App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol2015App theApp;
