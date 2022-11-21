
// Kol2017View.cpp : implementation of the CKol2017View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2017.h"
#endif

#include "Kol2017Doc.h"
#include "Kol2017View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol2017View

IMPLEMENT_DYNCREATE(CKol2017View, CView)

BEGIN_MESSAGE_MAP(CKol2017View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKol2017View construction/destruction

CKol2017View::CKol2017View() noexcept
{
	back = new DImage();
	back->Load(CString("Back2.jpg"));
	body = new DImage();
	body->Load(CString("Body.png"));
	leg1 = new DImage();
	leg1->Load(CString("Leg1.png"));
	leg2 = new DImage();
	leg2->Load(CString("Leg2.png"));
	leg3 = new DImage();
	leg3->Load(CString("Leg3.png"));
	angle = new float[8];
	angle[0] = 0; // = { 0, 10, 20, 10, 0, -10, -20, -10 };
	angle[1] = 10 * 3.14151 / 180;
	angle[2] = 20 * 3.14151 / 180;
	angle[3] = 10 * 3.14151 / 180;
	angle[4] = 0;
	angle[5] = -10 * 3.14151 / 180;
	angle[6] = -20 * 3.14151 / 180;
	angle[7] = -10 * 3.14151 / 180;
}

CKol2017View::~CKol2017View()
{
	delete[] angle;
	delete back;
	delete body;
	delete leg1;
	delete leg2;
	delete leg3;
}

BOOL CKol2017View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2017View printing

BOOL CKol2017View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2017View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2017View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2017View diagnostics

#ifdef _DEBUG
void CKol2017View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2017View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2017Doc* CKol2017View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2017Doc)));
	return (CKol2017Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2017View message handlers



void CKol2017View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT) {
		i = (i + 1) % 8;
		x += 20;
		Invalidate();
	}
	if (nChar == VK_LEFT) {
		i = (i +8 - 1) % 8;
		x -= 20;
		Invalidate();
	}
	if (nChar == VK_UP) {
		scale += 0.10;
		Invalidate();
	}
	if (nChar == VK_DOWN) {
		scale -= 0.10;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKol2017View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
void CKol2017View::OnDraw(CDC* pDC)
{
	CKol2017Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect client;
	GetClientRect(&client);

	CDC* mem = new CDC();
	mem->CreateCompatibleDC(pDC);
	CBitmap* bm = new CBitmap();
	bm->CreateCompatibleBitmap(pDC, client.Width(), client.Height());
	CBitmap* old = mem->SelectObject(bm);
	int mode = mem->SetGraphicsMode(GM_ADVANCED);

	DrawMem(mem);

	pDC->BitBlt(0, 0, client.Width(), client.Height(), mem, 0, 0, SRCCOPY);

	mem->SetGraphicsMode(mode);
	mem->SelectObject(old);
	mem->DeleteDC();
	delete mem;
	delete bm;
}

void CKol2017View::DrawMem(CDC* pDC)
{
	CRect client;
	GetClientRect(&client);
	DrawScene(pDC, client);
}

void CKol2017View::DrawTransparent(CDC* pDC, DImage* img)
{
	int w = img->Width();
	int h = img->Height();
	CRect rect(0, 0, w, h);
	CDC* src = new CDC();
	CDC* dst = new CDC();
	src->CreateCompatibleDC(pDC);
	dst->CreateCompatibleDC(pDC);

	CBitmap* srcb = new CBitmap();
	srcb->CreateCompatibleBitmap(pDC, w, h);
	CBitmap* dstb = new CBitmap();
	dstb->CreateBitmap(w, h, 1, 1, nullptr);

	CBitmap* olds = src->SelectObject(srcb);
	CBitmap* oldd = dst->SelectObject(dstb);

	img->Draw(src, rect, rect);

	src->SetBkColor(src->GetPixel(0, 0));

	dst->BitBlt(0, 0, w, h, src, 0, 0, SRCCOPY);

	src->SetBkColor(RGB(0, 0, 0));
	src->SetTextColor(RGB(255, 255, 255));

	src->BitBlt(0, 0, w, h, dst,0, 0, SRCAND);

	pDC->BitBlt(0, 0, w, h, dst, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, src, 0, 0, SRCPAINT);

	
	src->SelectObject(olds);
	dst->SelectObject(oldd);
	delete srcb;
	delete dstb;
	src->DeleteDC();
	delete src;
	dst->DeleteDC();
	delete dst;
}

void CKol2017View::LoadIdentity(CDC* pDC)
{
}

void CKol2017View::Translate(CDC* pDC, float dX, float dY, bool rm)
{
	XFORM form = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&form, rm ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2017View::Rotate(CDC* pDC, float angle, bool rm)
{
	float cosa = cos(angle);
	float sina = sin(angle);
	XFORM form = {cosa, sina, -sina, cosa, 0, 0};
	pDC->ModifyWorldTransform(&form, rm ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void CKol2017View::Scale(CDC* pDC, float sX, float sY, bool rm)
{
	XFORM form = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&form, rm ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2017View::DrawScene(CDC* pDC, CRect rect)
{
	XFORM form;
	pDC->GetWorldTransform(&form);
	back->Draw(pDC, CRect(0, 0, back->Width(), back->Height()), rect);
	Scale(pDC, scale, scale);
	Translate(pDC, 100 + x, 100);
	DrawTransparent(pDC, body);

	DrawLeg(pDC, angle[(i) % 8], 52, 168);
	DrawLeg(pDC, -angle[(i) % 8], 52, 168);

	DrawLeg(pDC, angle[(i + 3) % 8], 270, 168);
	DrawLeg(pDC, -angle[(i + 3) % 8], 270, 168);

	pDC->SetWorldTransform(&form);
}

void CKol2017View::DrawLeg(CDC* pDC, double alpha, double dx, double dy)
{
	XFORM form;
	pDC->GetWorldTransform(&form);
	Translate(pDC, dx, dy);

	Translate(pDC, 28, 29);
	Rotate(pDC, alpha);
	Translate(pDC, -28, -29);

	DrawTransparent(pDC, leg1);

	Translate(pDC, 0, leg1->Height());

	Translate(pDC, 0, -40);

	Translate(pDC, 28, 29);
	Rotate(pDC, -alpha);
	Translate(pDC, -28, -29);
	DrawTransparent(pDC, leg2);

	Translate(pDC, -leg3->Width()/4, leg2->Height() - 29);

	DrawTransparent(pDC, leg3);

	pDC->SetWorldTransform(&form);
}
