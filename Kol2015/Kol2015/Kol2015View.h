
// Kol2015View.h : interface of the CKol2015View class
//

#pragma once
#include "DImage.h"
#define toRad(x) x * 3.14151 / 180

class CKol2015View : public CView
{
protected: // create from serialization only
	CKol2015View() noexcept;
	DECLARE_DYNCREATE(CKol2015View)

// Attributes
public:
	CKol2015Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CKol2015View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	//KOLOKVIJUM

	void DrawPackman(CDC* pDC, CRect rect, float angle);
	void DrawGhost(CDC* pDC, CRect rect);
	void DrawMem(CDC* pDC);
	void Rotate(CDC* pDC, float angle, bool rm = false);
	void Translate(CDC* pDC, float dX, float dY, bool rm = false);
	void Scale(CDC* pDC, float xS, float yS, bool rm = false);
	void Mirror(CDC* pDC, bool mx, bool my, bool rm = false);
	DImage* blue;
	POINT packman, ghost;
	int angle = 45;
	int orientation = 0;
};

#ifndef _DEBUG  // debug version in Kol2015View.cpp
inline CKol2015Doc* CKol2015View::GetDocument() const
   { return reinterpret_cast<CKol2015Doc*>(m_pDocument); }
#endif

