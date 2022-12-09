
// Kol2013View.h : interface of the CKol2013View class
//

#pragma once
#include "DImage.h"

class CKol2013View : public CView
{
protected: // create from serialization only
	CKol2013View() noexcept;
	DECLARE_DYNCREATE(CKol2013View)

// Attributes
public:
	CKol2013Doc* GetDocument() const;

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
	virtual ~CKol2013View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	void DrawBlade(CDC* pDC, int size);
	void Translate(CDC* pDC, double x, double y, bool rightMul = false);
	void Rotate(CDC* pDC, float angle, bool rightMul = false);
	void DrawStar(CDC* pDC, int size);
	void MyDraw(CDC* pDC);
	DImage* sky, * bark;

	float angle = 0.5;
	int x = 100;
	int y = 200;
	int precnik = 40;


	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in Kol2013View.cpp
inline CKol2013Doc* CKol2013View::GetDocument() const
   { return reinterpret_cast<CKol2013Doc*>(m_pDocument); }
#endif

