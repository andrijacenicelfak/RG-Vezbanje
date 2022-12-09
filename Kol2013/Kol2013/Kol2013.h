
// Kol2013.h : main header file for the Kol2013 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol2013App:
// See Kol2013.cpp for the implementation of this class
//

class CKol2013App : public CWinApp
{
public:
	CKol2013App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol2013App theApp;
