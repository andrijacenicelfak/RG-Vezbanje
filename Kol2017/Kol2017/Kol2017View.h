
// Kol2017View.h : interface of the CKol2017View class
//

#pragma once
#include "DImage.h"


class CKol2017View : public CView
{
protected: // create from serialization only
	CKol2017View() noexcept;
	DECLARE_DYNCREATE(CKol2017View)

// Attributes
public:
	CKol2017Doc* GetDocument() const;

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
	virtual ~CKol2017View();
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
	DImage* body, * back, * leg1, * leg2, * leg3;

	float* angle;
	int i = 0;
	int x = 0;
	float scale = 1;

	void DrawMem(CDC* pDC);

	void DrawTransparent(CDC *pDC, DImage* img);

	void LoadIdentity(CDC* pDC);
	void Translate(CDC* pDC, float dX, float dY, bool rm = false);
	void Rotate(CDC* pDC, float angle, bool rm = false);
	void Scale(CDC* pDC, float sX, float sY, bool rm = false);

	void DrawScene(CDC* pDC, CRect rect);
	void DrawLeg(CDC* pDC, double alpha, double dx, double dy);

};

#ifndef _DEBUG  // debug version in Kol2017View.cpp
inline CKol2017Doc* CKol2017View::GetDocument() const
   { return reinterpret_cast<CKol2017Doc*>(m_pDocument); }
#endif

