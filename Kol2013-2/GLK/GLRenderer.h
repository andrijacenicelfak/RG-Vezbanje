#pragma once

class CGLRenderer
{
public:
	float a1 = 0, a2 = 0;
	UINT tex[4];

	//float* donjiV, * donjiN, * donjiT, * terasaV, * terasaN, * terasaT, * srednjiV, * srednjiN, * srednjiT, * gornjiV, * gornjiN, * gornjiT;

	float** toranj;

	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawPart(int index);
	void FillVA(float* buff, float a, float b, float h);
	void FillVANorm(float* buff, float a, float b, float h);
	void FillVTex(float* buff, float x1, float x2);

	void DrawAxis();
	void OnKeyDown(UINT nChar);
	UINT LoadTexture(CString file);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
