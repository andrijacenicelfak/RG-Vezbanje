#pragma once

class CGLRenderer
{
public:

	float a1 = 45 * 3.14159 / 180, a2 = 45 * 3.14159 / 180;
	float rotK[3] = { 0, 0, 0 };
	float x, y, z;
	UINT tex;

	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawRubikCube(double a, int count);
	void DrawASide(float a, float s, float t);
	void DrawCube(float a, float s, float t);
	void DrawAxis();
	void OnKeyDown(UINT nChar);
	UINT LoadTexture(CString file);
	void SetMaterial(float r, float g, float b);
	void SetWhiteLight();

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
