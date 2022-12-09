
// Kol2012View.h : interface of the CKol2012View class
//

#pragma once


class CKol2012View : public CView
{
protected: // create from serialization only
	CKol2012View() noexcept;
	DECLARE_DYNCREATE(CKol2012View)

// Attributes
public:
	CKol2012Doc* GetDocument() const;

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
	virtual ~CKol2012View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void Trapez(CDC* pDC, int a, int b, int h);
	void Translate(CDC* pDC, float x, float y, bool mr = false);
	void Rotate(CDC* pDC, float a, bool mr = false);
	void DrawGear(CDC* pDC, int spokeCount, int spokeWidth);
	void DrawScene(CDC* pDC);
	float GearR(int spokeCount, int spokeWidth);
	float angle = 0;

};

#ifndef _DEBUG  // debug version in Kol2012View.cpp
inline CKol2012Doc* CKol2012View::GetDocument() const
   { return reinterpret_cast<CKol2012Doc*>(m_pDocument); }
#endif

