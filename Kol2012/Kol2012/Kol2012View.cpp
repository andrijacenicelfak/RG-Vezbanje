
// Kol2012View.cpp : implementation of the CKol2012View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2012.h"
#endif

#include "Kol2012Doc.h"
#include "Kol2012View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol2012View

IMPLEMENT_DYNCREATE(CKol2012View, CView)

BEGIN_MESSAGE_MAP(CKol2012View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CKol2012View construction/destruction

CKol2012View::CKol2012View() noexcept
{
	// TODO: add construction code here

}

CKol2012View::~CKol2012View()
{
}

BOOL CKol2012View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2012View drawing



// CKol2012View printing

BOOL CKol2012View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2012View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2012View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2012View diagnostics

#ifdef _DEBUG
void CKol2012View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2012View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2012Doc* CKol2012View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2012Doc)));
	return (CKol2012Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2012View message handlers


BOOL CKol2012View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CKol2012View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_UP) {
		angle += 3.14159 / 180 *20;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void CKol2012View::OnDraw(CDC* pDC)
{
	CKol2012Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect client;
	GetClientRect(&client);

	CBitmap* bm = new CBitmap();
	bm->CreateCompatibleBitmap(pDC, client.Width(), client.Height());
	CDC* mem = new CDC();
	mem->CreateCompatibleDC(pDC);
	CBitmap* old = mem->SelectObject(bm);
	mem->SetGraphicsMode(GM_ADVANCED);

	DrawScene(mem);

	pDC->BitBlt(0, 0, client.Width(), client.Height(), mem, 0, 0, SRCCOPY);

	mem->SelectObject(old);
	mem->DeleteDC();
	delete mem;
	delete bm;
}


void CKol2012View::Trapez(CDC * pDC, int a, int b, int h)
{
	POINT tacke[4] = { {-a / 2, 0}, {-b / 2, h}, {b / 2, h}, {a/2, 0} };
	pDC->Polygon(tacke, 4);
}

void CKol2012View::Translate(CDC * pDC, float x, float y, bool mr)
{
	XFORM form = { 1, 0, 0, 1, x, y };
	pDC->ModifyWorldTransform(&form, mr ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2012View::Rotate(CDC * pDC, float a, bool mr)
{
	float s = sin(a);
	float c = cos(a);
	XFORM form = { c, s, -s, c, 0, 0 };
	pDC->ModifyWorldTransform(&form, mr ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
float CKol2012View::GearR(int spokeCount, int spokeWidth) {
	return spokeCount * spokeWidth * 1.4 / (2 * 3.14159) + spokeWidth/2;// +spokeWidth + spokeWidth / 8;
}

void CKol2012View::DrawGear(CDC * pDC, int spokeCount, int spokeWidth)
{

	float r = spokeCount * spokeWidth * 1.4 / (2 * 3.14159) + spokeWidth / 2;
	float r2 = r - spokeWidth / 2; // efektivni

	CPen isprekp(PS_DASH, 1, RGB(0, 0, 0));
	CPen* oldp = pDC->SelectObject(&isprekp);
	
	CBrush* oldb = (CBrush*)pDC->SelectStockObject(HOLLOW_BRUSH);
	pDC->Ellipse(CRect(-r2, -r2, r2, r2));

	CBrush plavi(RGB(150, 150, 200));
	pDC->SelectObject(&plavi);
	CPen pen(PS_SOLID, 2, RGB(60, 60, 150));
	pDC->SelectObject(&pen);

	
	float step = 2 * 3.14159 / spokeCount;
	for (int i = 0; i < spokeCount; i++) {
		Rotate(pDC, step);
		XFORM f;
		pDC->GetWorldTransform(&f);
		Translate(pDC, r, 0);
		Rotate(pDC, -3.14159 / 2);
		Trapez(pDC, spokeWidth * 1.4, spokeWidth / 1.4, spokeWidth);
		pDC->SetWorldTransform(&f);
	}

	pDC->Ellipse(CRect(-r, -r, r, r));
	pDC->Rectangle(CRect(-spokeWidth, -spokeWidth, spokeWidth, spokeWidth));

	CFont f;
	f.CreateFont(18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, CString("Arial"));

	COLORREF bk = pDC->SetBkColor(RGB(150, 150, 200));

	CFont* oldf = pDC->SelectObject(&f);

	char tekst[] = "   zubaca";
	tekst[0] = '0' + spokeCount / 10;
	tekst[1] = '0' + spokeCount % 10;

	pDC->TextOut(-2 * spokeWidth, 20, CString(tekst));
	
	pDC->SetBkColor(bk);
	pDC->SelectObject(oldf);
	pDC->SelectObject(oldp);
	pDC->SelectObject(oldb);

}

void CKol2012View::DrawScene(CDC* pDC)
{
	CRect client;
	GetClientRect(&client);
	CBrush brush(BS_SOLID, RGB(255, 255, 255));
	pDC->FillRect(client, &brush);

	XFORM form;
	pDC->GetWorldTransform(&form);

	Translate(pDC, 300, 300);
	Rotate(pDC, angle / 20);
	DrawGear(pDC, 20, 20);
	Rotate(pDC, -angle / 20);
	XFORM prvi;
	pDC->GetWorldTransform(&prvi);
	float translateV = GearR(20, 20) + GearR(10, 20);
	Translate(pDC, -translateV / sqrt(2) - 15, -translateV / sqrt(2) - 15);
	Rotate(pDC, 14 * 3.14159 / 360 - angle/10);
	DrawGear(pDC, 10, 20);
	pDC->SetWorldTransform(&prvi);
	Translate(pDC, translateV + 20, 0);
	Rotate(pDC, -36 * 3.14159 / 360 - angle/10);
	DrawGear(pDC, 10, 20);

	pDC->SetWorldTransform(&form);

}
