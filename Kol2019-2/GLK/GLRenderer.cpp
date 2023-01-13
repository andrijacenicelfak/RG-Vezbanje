#include "StdAfx.h"
#include "GLRenderer.h"
#include "GL\gl.h"
#include "GL\glu.h"
#include "GL\glaux.h"
#include "GL\glut.h"
//#pragma comment(lib, "GL\\glut32.lib")
#include "DImage.h"
CGLRenderer::CGLRenderer(void)
{
}

CGLRenderer::~CGLRenderer(void)
{
}

bool CGLRenderer::CreateGLContext(CDC* pDC)
{
	PIXELFORMATDESCRIPTOR pfd ;
   	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
   	pfd.nSize  = sizeof(PIXELFORMATDESCRIPTOR);
   	pfd.nVersion   = 1; 
   	pfd.dwFlags    = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;   
   	pfd.iPixelType = PFD_TYPE_RGBA; 
   	pfd.cColorBits = 32;
   	pfd.cDepthBits = 24; 
   	pfd.iLayerType = PFD_MAIN_PLANE;
	
	int nPixelFormat = ChoosePixelFormat(pDC->m_hDC, &pfd);
	
	if (nPixelFormat == 0) return false; 

	BOOL bResult = SetPixelFormat (pDC->m_hDC, nPixelFormat, &pfd);
  	
	if (!bResult) return false; 

   	m_hrc = wglCreateContext(pDC->m_hDC); 

	if (!m_hrc) return false; 

	return true;	
}

void CGLRenderer::PrepareScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	//ucitaj texture
	//texExc, texFront, texLeft, texRight, texBack, texTop, TexBot;
	texExc = LoadTexture(CString("excavator.png"));
	texFront = LoadTexture(CString("front.jpg"));
	texLeft = LoadTexture(CString("left.jpg"));
	texRight = LoadTexture(CString("right.jpg"));
	texBack = LoadTexture(CString("back.jpg"));
	texTop = LoadTexture(CString("top.jpg"));
	texBot = LoadTexture(CString("bot.jpg"));
	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	float x = dist * cos(a1) * cos(a2);
	float y = dist * sin(a1);
	float z = dist * cos(a1) * sin(a2);

	gluLookAt(x, y, z, 0, 3, 0, 0, 1, 0);

	DrawExcavator();


	glPushMatrix();
	DrawEnvCube(100);
	glPopMatrix();

	DrawAxis();

	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::Reshape(CDC *pDC, int w, int h)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	
	// delete textures
	glDeleteTextures(1, &texBack);
	glDeleteTextures(1, &texFront);
	glDeleteTextures(1, &texLeft);
	glDeleteTextures(1, &texRight);
	glDeleteTextures(1, &texTop);
	glDeleteTextures(1, &texBot);
	glDeleteTextures(1, &texExc);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawEnvCube(double a)
{
	glPushMatrix();
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	
	glColor3f(1.0, 1.0, 1.0);
	glBindTexture(GL_TEXTURE_2D, texFront);
	glTranslatef(0, a / 2, 0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);
	}
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, texLeft);
	glRotatef(90, 0, 1, 0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);
	}
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, texBack);
	glRotatef(90, 0, 1, 0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texRight);
	glRotatef(90, 0, 1, 0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texTop);
	glRotatef(90, 1, 0, 0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, texBot);
	glRotatef(-180, 1, 0, 0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 0);
		glVertex3f(-a / 2, a / 2, -a / 2);

		glTexCoord2f(0, 1);
		glVertex3f(-a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 1);
		glVertex3f(a / 2, -a / 2, -a / 2);

		glTexCoord2f(1, 0);
		glVertex3f(a / 2, a / 2, -a / 2);
	}
	glEnd();
	glDisable(GL_CULL_FACE);
	glPopMatrix();
}

void CGLRenderer::DrawExcavator()
{
	glPushMatrix();
	glTranslatef(-4, 0, 0);
	DrawBase();
	glTranslatef(4, 2, 0);
	glRotatef(ka1 * 180 / 3.14159, 0, 1, 0);
	glTranslatef(-2, 0, 0);
	DrawBody();
	glTranslatef(3, 1, -2.5);
	glRotatef(90 + ka2 * 180 / 3.14159, 0, 0, 1);
	glTranslatef(-1, -1, 0);
	DrawArm(1);
	glTranslatef(7, 1, 0);
	glRotatef(-90 + ka3 * 180 / 3.14159, 0, 0, 1);
	glTranslatef(-1, -1, 0);
	DrawArm(1.5);
	glTranslatef(7, 1, 0);
	glRotatef(-90 + ka4 * 180 / 3.14159, 0, 0, 1);
	glTranslatef(-1, -1, 0);
	DrawFork();

	glPopMatrix();
}

void CGLRenderer::DrawFork()
{
	glBindTexture(GL_TEXTURE_2D, texExc);

	int n = 6;
	POINTF points[] = {
		{0, 5},
		{0, 0},
		{8, 0},
		{8, 5},
		{7, 6},
		{1, 6}
	};
	POINTF tex[6];
	for (int i = 0; i < n; i++) {
		tex[i].x = (points[i].x) / 16.0;
		tex[i].y = (6.0 - points[i].y) / 16.0;
		points[i].x /= 2;
		points[i].y /= 2;
	}

	DrawExtrudedPolygon(points, tex, n, 1, 0.7, 0.7, 0.7);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawArm(double zh)
{
	glBindTexture(GL_TEXTURE_2D, texExc);

	int n = 6;
	POINTF points[] = {
		{0, 3},
		{0, 1},
		{1, 0},
		{16, 1},
		{16, 3},
		{1, 4},
	};
	POINTF tex[6];
	for (int i = 0; i < n; i++) {
		tex[i].x = (points[i].x) / 16.0;
		tex[i].y = (12.0 - points[i].y) / 16.0;
		points[i].x /= 2;
		points[i].y /= 2;
	}

	DrawExtrudedPolygon(points, tex, n, zh, 0.96, 0.5, 0.12);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void CGLRenderer::DrawBody()
{
	glBindTexture(GL_TEXTURE_2D, texExc);
	int n = 5;
	POINTF points[] = {
		{0, 8},
		{0, 0},
		{8, 0},
		{8, 4},
		{4, 8},
	};
	POINTF tex[5];
	for (int i = 0; i < n; i++) {
		tex[i].x = (8 + points[i].x) / 16.0;
		tex[i].y = (8.0 - points[i].y) / 16.0;
		points[i].x /= 2;
		points[i].y /= 2;
	}

	DrawExtrudedPolygon(points, tex, n, 4, 0.96, 0.5, 0.12);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawBase()
{
	glBindTexture(GL_TEXTURE_2D, texExc);
	int n = 8;
	POINTF points[] = {
		{0, 3},
		{0, 1},
		{1, 0},
		{15, 0},
		{16, 1},
		{16, 3},
		{15, 4},
		{1, 4}
	};
	POINTF tex[8];
	for (int i = 0; i < n; i++) {
		tex[i].x = (points[i].x)/16.0;
		tex[i].y = (16.0 - points[i].y)/16.0;
		points[i].x /= 2;
		points[i].y /= 2;
	}

	DrawExtrudedPolygon(points, tex, n, 5, 0.3, 0.3, 0.3);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawExtrudedPolygon(POINTF* points, POINTF* texCoords, int n, float zh, float r, float g, float b)
{
	glPushMatrix();

	glTranslatef(0, 0, -zh / 2);
	DrawPolygon(points, texCoords, n);
	glTranslatef(0, 0, zh);
	DrawPolygon(points, texCoords, n);

	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glBegin(GL_QUAD_STRIP);
	glColor3f(r, g, b);

	for (int i = 0; i <= n; i++) {
		glVertex3f(points[i % n].x, points[i % n].y, -zh / 2);
		glVertex3f(points[i % n].x, points[i % n].y, zh / 2);
	}

	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void CGLRenderer::DrawPolygon(POINTF* points, POINTF* texCoords, int n)
{
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0, 1.0, 1.0);
	for (int i = 0; i < n; i++) {
		glTexCoord2f(texCoords[i].x, texCoords[i].y);
		glVertex3f(points[i].x, points[i].y, 0);
	}
	glEnd();
}

void CGLRenderer::DrawAxis()
{
	glBegin(GL_LINES);

	glColor3f(1.0, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);

	glColor3f(0, 1.0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);

	glEnd();
}

void CGLRenderer::OnKeyDown(UINT nChar)
{
	if (nChar == '1') {
		this->ka1 += 0.05;
	}
	if (nChar == '2') {
		this->ka1 -= 0.05;
	}
	if (nChar == '3') {
		this->ka2 += 0.05;
	}
	if (nChar == '4') {
		this->ka2 -= 0.05;
	}
	if (nChar == '5') {
		this->ka3 += 0.05;
	}
	if (nChar == '6') {
		this->ka3 -= 0.05;
	}
	if (nChar == '7') {
		this->ka4 += 0.05;
	}
	if (nChar == '8') {
		this->ka4 -= 0.05;
	}
	if (nChar == '1') {
		this->ka1 += 0.05;
	}
	if (nChar == 109 && dist < 50) {
		this->dist += 0.5;
	}
	if (nChar == 107 && dist > 8) {
		this->dist -= 0.5;
	}
	if (nChar == VK_UP) {
		a1 += 0.05;
	}
	if (nChar == VK_DOWN) {
		a1 -= 0.05;
	}
	if (nChar == VK_LEFT) {
		a2 += 0.05;
	}
	if (nChar == VK_RIGHT) {
		a2 -= 0.05;
	}
}

UINT CGLRenderer::LoadTexture(CString file)
{
	UINT texID;
	DImage* img = new DImage();
	img->Load(file);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img->Width(), img->Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img->GetDIBBits());

	delete img;
	glBindTexture(GL_TEXTURE_2D, 0);
	return texID;
}
