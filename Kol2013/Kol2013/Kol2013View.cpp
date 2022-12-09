
// Kol2013View.cpp : implementation of the CKol2013View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2013.h"
#endif

#include "Kol2013Doc.h"
#include "Kol2013View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol2013View

IMPLEMENT_DYNCREATE(CKol2013View, CView)

BEGIN_MESSAGE_MAP(CKol2013View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2013View construction/destruction

CKol2013View::CKol2013View() noexcept
{
	this->sky = new DImage();
	sky->Load(CString("sky.jpg"));
	this->bark = new DImage();
	bark->Load(CString("bark.jpg"));
}

CKol2013View::~CKol2013View()
{
	delete sky;
	delete bark;
}

BOOL CKol2013View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}


// CKol2013View printing

BOOL CKol2013View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2013View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2013View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2013View diagnostics

#ifdef _DEBUG
void CKol2013View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2013View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2013Doc* CKol2013View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2013Doc)));
	return (CKol2013Doc*)m_pDocument;
}
#endif //_DEBUG


void CKol2013View::OnDraw(CDC* pDC)
{
	CKol2013Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect client;
	GetClientRect(&client);

	CDC* mem = new CDC();
	mem->CreateCompatibleDC(pDC);
	CBitmap* bp = new CBitmap();
	bp->CreateCompatibleBitmap(pDC,client.Width(), client.Height());
	int mode = mem->SetGraphicsMode(GM_ADVANCED);

	CBitmap* old = mem->SelectObject(bp);

	this->MyDraw(mem);

	pDC->BitBlt(0, 0, client.Width(), client.Height(), mem, 0, 0, SRCCOPY);

	mem->SetGraphicsMode(mode);
	mem->SelectObject(old);
	mem->DeleteDC();
	delete bp;
	delete mem;
}
void CKol2013View::MyDraw(CDC* pDC) {

	CRect client;
	GetClientRect(&client);
	sky->Draw(pDC, CRect(0, 0, sky->Width(), sky->Height()), CRect(0, 0, sky->Width(), sky->Height()));
	DrawStar(pDC, precnik);
	bark->Draw(pDC, CRect(0, 0, bark->Width(), bark->Height()), CRect(3 * client.Width() / 4, 0, client.Width(), client.Height()));
}

void CKol2013View::DrawBlade(CDC* pDC, int size)
{
	POINT tacke[4] = { {0, 0}, {size*4, 0}, {size, size} };
	CBrush brush(RGB(130, 130, 130));
	CBrush* old = pDC->SelectObject(&brush);

	pDC->Polygon(tacke, 3);

	CBrush brush2(RGB(180, 180, 180));
	pDC->SelectObject(&brush2);
	tacke[2] = { size, -size};
	pDC->Polygon(tacke, 3);

	CString name("Monotype Corsiva");

	CFont f;
	f.CreateFont(0.7 * size / 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, name);

	CFont* oldf = pDC->SelectObject(&f);
	pDC->SetTextColor(RGB(100, 100, 100));
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(230, 230, 230));
	pDC->TextOut(size - 2, -2 - size / 4 * 0.7, CString("Shuricane"));
	pDC->SetTextColor(RGB(80, 80, 80));

	pDC->TextOut(size, -size / 4 * 0.7, CString("Shuricane"));
	pDC->SelectObject(old);
	pDC->SelectObject(oldf);
}

void CKol2013View::Translate(CDC* pDC, double x, double y, bool rightMul)
{
	XFORM form = { 1, 0, 0, 1, x, y };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2013View::Rotate(CDC* pDC, float angle, bool rightMul)
{
	float msin = sin(angle);
	float mcos = cos(angle);
	XFORM form = { mcos, msin, -msin, mcos, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMul ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2013View::DrawStar(CDC* pDC, int size)
{
	XFORM form;
	pDC->GetWorldTransform(&form);

	Translate(pDC, x, y);
	Rotate(pDC, this->angle);
	DrawBlade(pDC, size);
	Rotate(pDC, 3.14159 / 2);
	DrawBlade(pDC, size);
	Rotate(pDC, 3.14159 / 2);
	DrawBlade(pDC, size);
	Rotate(pDC, 3.14159 / 2);
	DrawBlade(pDC, size);

	pDC->SetWorldTransform(&form);
}

BOOL CKol2013View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CKol2013View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 'W') {
		CRect client;
		GetClientRect(&client);
		if (x + 10.0 < 3.0/4.0 * client.Width() - 4* 9 * precnik / 10) {
			x += 10;
			angle += 10 * 3.14159 / 360;
			Invalidate();
		}
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
