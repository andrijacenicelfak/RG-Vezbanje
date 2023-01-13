#pragma once
#define toDeg 180 / 3.14519
#define toRad 3.14519 / 180
class CGLRenderer
{
public:

	float a1 = 1.0, a2 = 1.0, a3 = 0, dist = 30, height = 10;
	UINT texBrick, texEnv;
	CGLRenderer(void);
	virtual ~CGLRenderer(void);
		
	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawPaddle(double lenght, double width);
	void DrawCone(double r, double h, int n);
	void DrawTube(double r1, double r2, double h, int n);
	void DrawCube(double a);
	void DrawAxis();

	void OnKeyDown(UINT nChar);
	UINT LoadTexture(CString file);

protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 

};
