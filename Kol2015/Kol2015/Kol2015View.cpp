
// Kol2015View.cpp : implementation of the CKol2015View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kol2015.h"
#endif

#include "Kol2015Doc.h"
#include "Kol2015View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKol2015View

IMPLEMENT_DYNCREATE(CKol2015View, CView)

BEGIN_MESSAGE_MAP(CKol2015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CKol2015View construction/destruction

CKol2015View::CKol2015View() noexcept
{
	blue = new DImage();
	blue->Load(CString("blue.png"));
	packman = { 400,400 };
	ghost = { 200, 120 };
}

CKol2015View::~CKol2015View()
{
	delete blue;
}

BOOL CKol2015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CKol2015View drawing

void CKol2015View::OnDraw(CDC* pDC)
{
	CKol2015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect client;
	GetClientRect(&client);

	CDC* mem = new CDC();
	mem->CreateCompatibleDC(pDC);
	CBitmap* bm = new CBitmap(), * old;
	bm->CreateCompatibleBitmap(pDC, client.Width(), client.Height());
	old = mem->SelectObject(bm);
	int mode = mem->SetGraphicsMode(GM_ADVANCED);
	DrawMem(mem);
	mem->SetGraphicsMode(mode);
	pDC->BitBlt(0, 0, client.Width(), client.Height(), mem, 0, 0, SRCCOPY);

	mem->SelectObject(old);
	delete bm;
	mem->DeleteDC();
	delete mem;
}


// CKol2015View printing

BOOL CKol2015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CKol2015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CKol2015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CKol2015View diagnostics

#ifdef _DEBUG
void CKol2015View::AssertValid() const
{
	CView::AssertValid();
}

void CKol2015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKol2015Doc* CKol2015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKol2015Doc)));
	return (CKol2015Doc*)m_pDocument;
}
#endif //_DEBUG


// CKol2015View message handlers


void CKol2015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_RIGHT) {
		angle = (angle + 10)%90;
		orientation = 0;
		packman.x += 10;
		Invalidate();
	}
	if (nChar == VK_UP) {
		angle = (angle + 10) % 90;
		orientation = 3;
		packman.y -= 10;
		Invalidate();
	}
	if (nChar == VK_LEFT) {
		angle = (angle + 10) % 90;
		orientation = 2;
		packman.x -= 10;
		Invalidate();
	}
	if (nChar == VK_DOWN) {
		angle = (angle + 10) % 90;
		orientation = 1;
		packman.y += 10;
		Invalidate();
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CKol2015View::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CKol2015View::DrawPackman(CDC* pDC, CRect rect, float angle)
{
	int x = rect.TopLeft().x;
	int y = rect.TopLeft().y;
	int w = rect.Width();
	int h = rect.Height();
	int r = w / 2;
	XFORM form;
	pDC->GetWorldTransform(&form);


	Translate(pDC, x - w / 2, y - h / 2);
	Rotate(pDC, orientation * 3.14151 / 2);
	COLORREF zuta = RGB(255, 255, 0);

	CBrush* b = new CBrush(zuta);
	CPen* p = new CPen(PS_SOLID, 1, RGB(0,0,0));
	CBrush* old = pDC->SelectObject(b);
	CPen* oldp = pDC->SelectObject(p);

	int mouth = r * sin(angle);
	mouth = mouth * (mouth < 0 ? -1 : 1);
	CRect* pack = new CRect(-w / 2, -h / 2, w / 2, h / 2);
	POINT p1 = { w / 2, mouth };
	POINT p2 = { w / 2, -mouth };
	pDC->Pie(pack, p2, p1);

	CRect oko(0, 0, w / 8, h / 8);
	CBrush* black = new CBrush(RGB(0, 0, 0));
	pDC->SelectObject(black);
	Mirror(pDC, false, orientation == 2);
	Translate(pDC, w / 7 , -h / 3);
	pDC->Ellipse(&oko);

	pDC->SetWorldTransform(&form);
	delete pack;
	
	pDC->SelectObject(old);
	pDC->SelectObject(oldp);
	delete b;
	delete p;
	delete black;
}

void CKol2015View::DrawGhost(CDC* pDC, CRect rect)
{
	int x = rect.TopLeft().x;
	int y = rect.TopLeft().y;
	int w = rect.Width();
	int h = rect.Height();
	int r = w / 2;

	XFORM form;
	pDC->GetWorldTransform(&form);
	Translate(pDC, x, y);
	COLORREF crvena = RGB(255, 0, 0);

	CBrush* b = new CBrush(crvena);
	CPen* p = new CPen(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush* old = pDC->SelectObject(b);
	CPen* oldp = pDC->SelectObject(p);

	float rl = r / 6.0;
	POINT curr = { 0, 0 };
	pDC->BeginPath();

	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(CRect(-r, -r, r, r), {-r, 0}, {r, 0});
	
	pDC->LineTo(r, r);
	curr = { r, r };
	pDC->ArcTo(CRect(-rl + curr.x, -rl + curr.y, rl + curr.x, rl + curr.y), { curr.x ,(int) (curr.y + rl) }, { (int)(curr.x - rl), curr.y });

	for (int i = 0; i < 2; i++) {
		pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
		curr = { int(curr.x - 2 * rl), curr.y };
		pDC->ArcTo(CRect(-rl + curr.x, -rl + curr.y, rl + curr.x, rl + curr.y), { curr.x ,(int)(curr.y + rl) }, { (int)(curr.x - rl), curr.y });
		pDC->SetArcDirection(AD_CLOCKWISE);

		curr = { int(curr.x - 2 * rl), curr.y };
		pDC->ArcTo(CRect(-rl + curr.x, -rl + curr.y, rl + curr.x, rl + curr.y), { curr.x ,(int)(curr.y + rl) }, { (int)(curr.x - rl), curr.y });

	}
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	curr = { int(curr.x - 2 * rl), curr.y };
	pDC->ArcTo(CRect(-rl + curr.x, -rl + curr.y, rl + curr.x, rl + curr.y), { curr.x ,(int)(curr.y + rl) }, { (int)(curr.x - rl), curr.y });

	pDC->SetArcDirection(AD_CLOCKWISE);

	curr = { int(curr.x - 2 * rl), (int)(curr.y) };
	pDC->ArcTo(CRect(-rl + curr.x, -rl + curr.y, rl + curr.x, rl + curr.y), { int(curr.x + rl) ,curr.y }, { (int)(curr.x), (int)(curr.y + rl) });
	pDC->LineTo({ -r, 0 });

	pDC->EndPath();

	pDC->FillPath();

	pDC->SetWorldTransform(&form);
}

void CKol2015View::DrawMem(CDC* pDC)
{
	CRect client;
	GetClientRect(&client);
	blue->Draw(pDC, CRect(0, 0, blue->Width(), blue->Height()), client);
	//pDC->Rectangle(client); //TODO DELETE
	DrawPackman(pDC, CRect(packman, POINT{ packman.x + 50, packman.y + 50 }), toRad(angle - 45));
	DrawGhost(pDC, CRect(ghost, POINT{ ghost.x + 200, ghost.y + 200 }));
}

void CKol2015View::Rotate(CDC* pDC, float angle, bool rm)
{
	float sa = sin(angle);
	float ca = cos(angle);
	XFORM form = {ca, sa, -sa, ca, 0, 0};
	pDC->ModifyWorldTransform(&form, rm ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2015View::Translate(CDC* pDC, float dX, float dY, bool rm)
{
	XFORM form = { 1, 0, 0, 1, dX, dY };
	pDC->ModifyWorldTransform(&form, rm ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}
void CKol2015View::Scale(CDC* pDC, float xS, float yS, bool rm)
{
	XFORM form = { xS, 0, 0, yS, 0, 0 };
	pDC->ModifyWorldTransform(&form, rm ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void CKol2015View::Mirror(CDC* pDC, bool mx, bool my, bool rm)
{
	Scale(pDC, mx ? -1 : 1, my ? -1 : 1, rm);
}
