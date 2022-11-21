
// Kol2018View.cpp : implementation of the CKol2018View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2018.h"
#endif

#include "Kol2018Doc.h"
#include "Kol2018View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol2018View

IMPLEMENT_DYNCREATE(CKol2018View, CView)

BEGIN_MESSAGE_MAP(CKol2018View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKol2018View construction/destruction

CKol2018View::CKol2018View() noexcept
{
	bager = new DImage();
	bager->Load(CString("bager.png"));
	arm1 = new DImage();
	arm1->Load(CString("arm1.png"));
	arm2 = new DImage();
	arm2->Load(CString("arm2.png"));
	pozadina = new DImage();
	pozadina->Load(CString("pozadina.png"));
	viljuska = GetEnhMetaFile(CString("viljuska.emf"));
}

CKol2018View::~CKol2018View()
{
	delete bager;
	delete arm1;
	delete arm2;
	delete pozadina;
	DeleteEnhMetaFile(viljuska);
}

BOOL CKol2018View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2018View drawing

void CKol2018View::OnDraw(CDC* pDC)
{
	CKol2018Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect client;
	GetClientRect(&client);
	CDC* mem = new CDC();
	mem->CreateCompatibleDC(pDC);
	CBitmap memBitmap;
	memBitmap.CreateCompatibleBitmap(pDC, client.Width(), client.Height());
	CBitmap* old = mem->SelectObject(&memBitmap);
	
	int mode = mem->SetGraphicsMode(GM_ADVANCED);
	
	DrawMem(mem);

	mem->SetGraphicsMode(mode);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), mem, 0, 0, SRCCOPY);

	mem->SelectObject(old);
	mem->DeleteDC();
	delete mem;
}


// CKol2018View printing

BOOL CKol2018View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2018View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2018View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2018View diagnostics

#ifdef _DEBUG
void CKol2018View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2018View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2018Doc* CKol2018View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2018Doc)));
	return (CKol2018Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2018View message handlers

void CKol2018View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM form = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2018View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float s = sin(angle);
	float c = cos(angle);
	XFORM form = { c, s, -s, c, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2018View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply)
{
	XFORM form = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2018View::DrawBackground(CDC* pDC)
{
	CRect client;
	GetClientRect(&client);
	int x, y, w, h;
	x = (client.Width() - pozadina->Width()) / 2;
	w = x + pozadina->Width();
	y = client.Height() - pozadina->Height();
	h = y + pozadina->Height();
	pozadina->Draw(pDC, CRect(0, 0, pozadina->Width(), pozadina->Height()), CRect(x, y, w, h));
}

void CKol2018View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	int w = pImage->Width();
	int h = pImage->Height();
	CRect rect(0, 0, w, h);

	CDC* srcDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);

	CDC* dstDC = new CDC();
	dstDC->CreateCompatibleDC(pDC);
	
	CBitmap* src = new CBitmap();
	src->CreateCompatibleBitmap(pDC, w, h);

	CBitmap* dst = new CBitmap();
	dst->CreateBitmap(w, h, 1, 1, nullptr);

	CBitmap* srcOld = srcDC->SelectObject(src);
	CBitmap* dstOld = dstDC->SelectObject(dst);

	pImage->Draw(srcDC, rect, rect);

	srcDC->SetBkColor(srcDC->GetPixel(0, 0));

	dstDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCCOPY);
	
	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);

	pDC->BitBlt(0, 0, w, h, dstDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, srcDC, 0, 0, SRCPAINT);


	dstDC->SelectObject(dstOld);
	srcDC->SelectObject(srcOld);

	dstDC->DeleteDC();
	delete dstDC;

	srcDC->DeleteDC();
	delete srcDC;

	delete src;
	delete dst;
}

void CKol2018View::DrawBody(CDC* pDC)
{
	CRect client;
	GetClientRect(&client);
	int x = client.Width() - bager->Width() - 30;
	int y = client.Height() - bager->Height() - 5;
	Translate(pDC, x, y);
	DrawImgTransparent(pDC, bager);
	Translate(pDC, 15, bager->Height() / 4 + 25);
}

void CKol2018View::DrawArm1(CDC* pDC)
{
	Translate(pDC, 58, 68);
	Rotate(pDC, a1 - PI/2);
	Translate(pDC, -58, -68);

	DrawImgTransparent(pDC, arm1);

	Translate(pDC, 309- 36, 58/2 - 6);
}

void CKol2018View::DrawArm2(CDC* pDC)
{
	Translate(pDC, 36, 40);
	Rotate(pDC, a2 - PI / 2);
	Translate(pDC, -36, -40);

	DrawImgTransparent(pDC, arm2);

	Translate(pDC,272,40);
}

void CKol2018View::DrawFork(CDC* pDC)
{
	ENHMETAHEADER header;
	GetEnhMetaFileHeader(viljuska, sizeof(ENHMETAHEADER), &header);
	int w = header.rclBounds.right - header.rclBounds.left;
	int h = header.rclBounds.bottom - header.rclBounds.top;
	Scale(pDC, 2.5, 2.5);
	Translate(pDC, -14, -20);
	Translate(pDC, 14, 20);
	Rotate(pDC, a3 - PI/2);
	Translate(pDC, -14, -20);
	PlayEnhMetaFile(pDC->m_hDC, viljuska, CRect(0, 0, w, h));
}

void CKol2018View::DrawExcavator(CDC* pDC)
{
	Translate(pDC, posX, 0);
	DrawBody(pDC);
	DrawArm1(pDC);
	DrawArm2(pDC);
	DrawFork(pDC);
}

void CKol2018View::DrawMem(CDC* pDC)
{
	XFORM form;
	pDC->GetWorldTransform(&form);
	DrawBackground(pDC);
	DrawExcavator(pDC);
	pDC->SetWorldTransform(&form);
}


void CKol2018View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == '1') {
		a1 += 10 * PI / 180;
		Invalidate();
	}
	if (nChar == '2') {
		a1 -= 10 * PI / 180;
		Invalidate();
	}
	if (nChar == '3') {
		a2 += 10 * PI / 180;
		Invalidate();
	}
	if (nChar == '4') {
		a2 -= 10 * PI / 180;
		Invalidate();
	}
	if (nChar == '6') {
		a3 += 10 * PI / 180;
		Invalidate();
	}
	if (nChar == '7') {
		a3 -= 10 * PI / 180;
		Invalidate();
	}
	if (nChar == VK_LEFT) {
		posX -= 4;
		Invalidate();
	}
	if (nChar == VK_RIGHT) {
		posX += 4;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKol2018View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
