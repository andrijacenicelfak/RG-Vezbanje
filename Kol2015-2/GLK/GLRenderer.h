#pragma once

class CGLRenderer
{
public:
	float a1 = 1.0, a2 = 1.0, a3 = 0;
	float dist = 10;
	bool svetlo = true;

	UINT texTSC[6];
	UINT texM[6];
	UINT texS[6];

	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawPatch(double R, int n);

	void OnKeyDown(UINT nChar);

	void DrawAxis();

	UINT LoadTexture(CString file);

	void DrawSphere(double R, int n, UINT* tex);

	void DrawEarth(double R, int tes);
	void DrawMoon(double R, int tes);
	void DrawSpace(double R, int tes);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
