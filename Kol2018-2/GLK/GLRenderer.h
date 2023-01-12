#pragma once
#define toDeg 180 / 3.14159
class CGLRenderer
{
public:
	UINT shipTex;
	UINT spaceTex[6];

	double FOV = 50;

	float a1 = 0, a2 = 0, dist = 5;

	bool EnabledLight = true;;

	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,
	bool OnKeyDown(UINT nChar);
	UINT LoadTexture(CString fileName);
	void DrawTriangle(float d1, float d2, float rep);
	void DrawShip();
	void DrawSpaceCube(double a);
	void DrawAxis();
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
