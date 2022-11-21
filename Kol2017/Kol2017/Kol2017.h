
// Kol2017.h : main header file for the Kol2017 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CKol2017App:
// See Kol2017.cpp for the implementation of this class
//

class CKol2017App : public CWinApp
{
public:
	CKol2017App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CKol2017App theApp;
