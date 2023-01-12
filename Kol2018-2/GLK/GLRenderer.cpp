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
	FOV = 50;
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

	// Load textures
	this->shipTex = LoadTexture(CString("ShipT1.png"));
	this->spaceTex[0] = LoadTexture(CString("front.jpg"));
	this->spaceTex[1] = LoadTexture(CString("left.jpg"));
	this->spaceTex[2] = LoadTexture(CString("right.jpg"));
	this->spaceTex[3] = LoadTexture(CString("back.jpg"));
	this->spaceTex[4] = LoadTexture(CString("top.jpg"));
	this->spaceTex[5] = LoadTexture(CString("bot.jpg"));

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
	float x = dist * cos(a2) * cos(a1), y = dist * sin(a2), z = dist * sin(a1) * cos(a2);
	gluLookAt(x,y,z, 0, 0, 0, 0, 1, 0);
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glTranslatef(x, y, z);
	DrawSpaceCube(1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glPopMatrix();

	GLfloat lightPos[] = { 0.0, 0.0, 1.0, 0.0 };
	GLfloat ambien[] = {1.0, 1.0, 1.0, 1.0};

	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambien);
	glLightfv(GL_LIGHT0, GL_SPECULAR, ambien);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambien);

	if (this->EnabledLight) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	glPushMatrix();
	glRotatef(90, 1, 0, 0);
	glTranslatef(-2.75, 0, 0);
	DrawShip();
	glPopMatrix();

	glDisable(GL_LIGHTING);

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
	gluPerspective((int)FOV, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	
	////delete textures
	glDeleteTextures(6, spaceTex);
	glDeleteTextures(1, &shipTex);
	
	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

bool CGLRenderer::OnKeyDown(UINT nChar)
{
	if (nChar == VK_UP) {
		a2 += 0.05;
		return true;
	}
	if (nChar == VK_DOWN) {
		a2 -= 0.05;
		return true;
	}
	if (nChar == VK_LEFT) {
		a1 += 0.05;
		return true;
	}
	if (nChar == VK_RIGHT) {
		a1 -= 0.05;
		return true;
	}
	if (nChar == 109) {
		dist += 0.5;
		return true;
	}
	if (nChar == 107) {
		dist -= 0.5;
		return true;
	}
	if (nChar == 'S') {
		this->EnabledLight = !this->EnabledLight;
		return true;
	}
	
	return false;
}

UINT CGLRenderer::LoadTexture(CString fileName)
{
	UINT texID;
	DImage* img = new DImage();
	img->Load(fileName);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img->Width(), img->Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img->GetDIBBits());

	glBindTexture(GL_TEXTURE_2D, 0);

	delete img;
	return texID;
}

void CGLRenderer::DrawTriangle(float d1, float d2, float rep)
{
	float a1 = atan2(d2, d1);
	float d3 = sqrt(d1 * d1 + d2 * d2);
	float y = d1 * cos(a1) / d3;
	float x = d1 * sin(a1) / d3;

	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 1.0, 1.0);

	glNormal3f(0, 0, 1);

	glTexCoord2f(0.5*rep, 0);
	glVertex3f(0, 0, 0);

	glTexCoord2f((0.5 + x)*rep, y*rep);
	glVertex3f(d1, 0, 0);

	glTexCoord2f(0.5*rep, 1.0 * rep);
	glVertex3f(d1, d2, 0);


	glEnd();
}

void CGLRenderer::DrawShip()
{
	float d1 = 5.8, d2 = 2.15;
	float angle = atan2(d2, d1);

	glPushMatrix();
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-angle * toDeg, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, shipTex);
	DrawTriangle(d1, d2, 3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, -1, 1);
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-angle * toDeg, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, shipTex);
	DrawTriangle(d1, d2, 3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	glPushMatrix();

	glScalef(1, 1, -1);

	glPushMatrix();
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-angle * toDeg, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, shipTex);
	DrawTriangle(d1, d2, 3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	glPushMatrix();
	glScalef(1, -1, 1);
	glRotatef(-4.75, 0, 1, 0);
	glRotatef(13, 1, 0, 0);
	glRotatef(-angle * toDeg, 0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, shipTex);
	DrawTriangle(d1, d2, 3);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	glPopMatrix();
}

void CGLRenderer::DrawSpaceCube(double a)
{
	glPushMatrix();
	//front
	glBindTexture(GL_TEXTURE_2D, spaceTex[0]);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);

	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a / 2, a / 2);


	glTexCoord2f(1, 1);
	glVertex3f(a / 2, -a / 2, a / 2);

	glEnd();

	glRotatef(90, 0, 1, 0);
	// levo
	glBindTexture(GL_TEXTURE_2D, spaceTex[2]);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);

	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a / 2, a / 2);


	glTexCoord2f(1, 1);
	glVertex3f(a / 2, -a / 2, a / 2);

	glEnd();

	glRotatef(90, 0, 1, 0);
	// pozadi
	glBindTexture(GL_TEXTURE_2D, spaceTex[3]);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);

	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a / 2, a / 2);


	glTexCoord2f(1, 1);
	glVertex3f(a / 2, -a / 2, a / 2);

	glEnd();

	glRotatef(90, 0, 1, 0);
	// desno
	glBindTexture(GL_TEXTURE_2D, spaceTex[1]);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);

	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a / 2, a / 2);


	glTexCoord2f(1, 1);
	glVertex3f(a / 2, -a / 2, a / 2);

	glEnd();

	glRotatef(90, 1, 0, 0);
	glRotatef(-90, 0, 0, 1);
	// dole
	glBindTexture(GL_TEXTURE_2D, spaceTex[5]);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);

	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a / 2, a / 2);


	glTexCoord2f(1, 1);
	glVertex3f(a / 2, -a / 2, a / 2);

	glEnd();

	glRotatef(2*90, 1, 0, 0);
	// dole
	glBindTexture(GL_TEXTURE_2D, spaceTex[4]);

	glBegin(GL_QUADS);

	glTexCoord2f(0, 1);
	glVertex3f(-a / 2, -a / 2, a / 2);

	glTexCoord2f(0, 0);
	glVertex3f(-a / 2, a / 2, a / 2);

	glTexCoord2f(1, 0);
	glVertex3f(a / 2, a / 2, a / 2);


	glTexCoord2f(1, 1);
	glVertex3f(a / 2, -a / 2, a / 2);

	glEnd();

	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, 0);
}

void CGLRenderer::DrawAxis()
{
	glBegin(GL_LINES);

	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);

	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);

	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);


	glEnd();

	glColor3f(1, 1, 1);


}
