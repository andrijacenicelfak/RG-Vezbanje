#pragma once

class CGLRenderer
{
public:
	float a1 = 1.0, a2 = 1.0, dist = 15, ka1 = 0, ka2 = 0, ka3 = 0, ka4 = 0;
	UINT texExc, texFront, texLeft, texRight, texBack, texTop, texBot;
	CGLRenderer(void);
	virtual ~CGLRenderer(void);

	bool CreateGLContext(CDC* pDC);			// kreira OpenGL Rendering Context
	void PrepareScene(CDC* pDC);			// inicijalizuje scenu,
	void Reshape(CDC* pDC, int w, int h);	// kod koji treba da se izvrsi svaki put kada se promeni velicina prozora ili pogleda i
	void DrawScene(CDC* pDC);				// iscrtava scenu
	void DestroyScene(CDC* pDC);			// dealocira resurse alocirane u drugim funkcijama ove klase,

	void DrawEnvCube(double a);
	void DrawExcavator();
	void DrawFork();
	void DrawArm(double zh);
	void DrawBody();
	void DrawBase();
	void DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b);
	void DrawPolygon(POINTF* points, POINTF* texCoords, int n);

	void DrawAxis();
	void OnKeyDown(UINT nChar);

	UINT LoadTexture(CString file);
protected:
	HGLRC	 m_hrc; //OpenGL Rendering Context 


};
