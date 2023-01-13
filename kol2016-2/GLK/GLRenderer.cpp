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

	//load textures

	texBrick = LoadTexture(CString("brick.png"));
	texEnv = LoadTexture(CString("env.png"));

	glEnable(GL_TEXTURE_2D);
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
	gluPerspective(40, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	
	// delete textures
	glDeleteTextures(1, &texBrick);
	glDeleteTextures(1, &texEnv);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawScene(CDC* pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);

	float x = dist * cos(a1) * cos(a2);
	float y = dist * sin(a1);
	float z = dist * cos(a1) * sin(a2);
	gluLookAt(x, y, z, 0, height, 0, 0, 1, 0);

	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glTranslatef(x, y, z);
	glBindTexture(GL_TEXTURE_2D, texEnv);
	DrawCube(2);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();


	float dir[] = { 1.0, 1.0, 0.0, 0.0 };
	float lig[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, lig);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lig);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lig);
	glLightfv(GL_LIGHT0, GL_POSITION, dir);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, texBrick);
	DrawTube(3.5, 2.5, height, 32);
	
	glTranslatef(0, height, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	DrawCone(3.8, 2, 32);
	glTranslatef(0, 0, 3.8);

	glRotatef(a3 * toDeg, 0, 0, 1);
	DrawPaddle(8, 1.5);
	glRotatef(90, 0, 0, 1);
	DrawPaddle(8, 1.5);
	glRotatef(90, 0, 0, 1);
	DrawPaddle(8, 1.5);
	glRotatef(90, 0, 0, 1);
	DrawPaddle(8, 1.5);
	
	glPopMatrix();

	glDisable(GL_LIGHTING);
	DrawAxis();
	
	glFlush();
	SwapBuffers(pDC->m_hDC);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawPaddle(double lenght, double width)
{
	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);

	glVertex3f(0, 0, 0);
	glVertex3f(width / 8, 0, 0);
	glVertex3f(width / 8, lenght / 8, 0);
	glVertex3f(0, lenght / 8, 0);

	glVertex3f(0, lenght / 8, 0);
	glVertex3f(width / 8, lenght / 8, 0);
	glVertex3f(width / 8, 2*lenght / 8, 0);
	glVertex3f(0, 2 * lenght / 8, 0);


	glVertex3f(0, 2 * lenght / 8, 0);
	glVertex3f(width, 2 * lenght / 8, 0);
	glVertex3f(width, lenght, 0);
	glVertex3f(0, lenght, 0);


	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex3f(width / 8, lenght / 8, 0);
	glVertex3f(width, 2*lenght / 8, 0);
	glVertex3f(width / 8, 2 * lenght / 8, 0);

	glNormal3f(0, 0, 1);


	glEnd();
}

void CGLRenderer::DrawCone(double r, double h, int n)
{
	double angleStep = 2 * 3.14159 / n;
	double angle = 0;
	glBegin(GL_TRIANGLE_FAN);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.6, 0.6, 0.6);
	glNormal3f(0, 1, 0);
	glVertex3f(0, h, 0);
	float y = h / r;
	for (int i = 0; i < n + 1; i++) {

		float x = cos(angle);
		float z = sin(angle);
		glNormal3f(x, y, z);
		glVertex3f(x * r, 0, z * r);
		angle += angleStep;
	}
	glEnd();


}

void CGLRenderer::DrawTube(double r1, double r2, double h, int n)
{
	double angleStep = 2 * 3.14159 / n;
	double angle = 0;
	glBegin(GL_QUAD_STRIP);
	glColor3f(1.0, 1.0, 1.0);
	double y = sin(atan2(r1 - r2, h));

	for (int i = 0; i < n + 1; i++) {

		float x = cos(angle);
		float z = sin(angle);
		glNormal3f(x, y, z);
		glTexCoord2f((double)i / n, 0);
		glVertex3f(x*r1, 0, z*r1);

		glTexCoord2f((double)i / n, 1);
		glVertex3f(x*r2, h, z*r2);
		angle += angleStep;
	}

	glEnd();
}

void CGLRenderer::DrawCube(double a)
{
	glColor3f(1.0, 1.0, 1.0);

	glPushMatrix();

	glBegin(GL_QUADS);
	{
		glTexCoord2f(0, 2.0 / 3.0);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(1.0 / 4.0, 2.0 / 3.0);
		glVertex3f(-a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0 / 4.0, 1.0 / 3.0);
		glVertex3f(a / 2, -a / 2, a / 2);

		glTexCoord2f(0, 1.0 / 3.0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();

	glRotatef(-90, 0, 1, 0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(2.0 / 4.0, 1);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(2.0 / 4.0, 2.0 / 3.0);
		glVertex3f(-a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0 / 4.0, 2.0/3.0);
		glVertex3f(a / 2, -a / 2, a / 2);
		
		glTexCoord2f(1.0/4.0, 1.0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();

	glRotatef(-90, 0, 1, 0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(3.0 / 4.0, 1.0 / 3.0);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(2.0 / 4.0, 1.0 / 3.0);
		glVertex3f(-a / 2, -a / 2, a / 2);

		glTexCoord2f(2.0 / 4.0, 2.0 / 3.0);
		glVertex3f(a / 2, -a / 2, a / 2);

		glTexCoord2f(3.0 / 4.0, 2.0 / 3.0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();


	glRotatef(-90, 0, 1, 0);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(1.0 / 4.0, 0);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(1.0 / 4.0, 1.0 / 3.0);
		glVertex3f(-a / 2, -a / 2, a / 2);

		glTexCoord2f(2.0 / 4.0, 1.0 / 3.0);
		glVertex3f(a / 2, -a / 2, a / 2);

		glTexCoord2f(2.0 / 4.0, 0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();

	glPopMatrix();

	glPushMatrix();

	glRotatef(90, 1, 0, 0);

	glBegin(GL_QUADS);
	{
		glTexCoord2f(1.0/4.0, 2.0 / 3.0);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(2.0 / 4.0, 2.0 / 3.0);
		glVertex3f(-a / 2, -a / 2, a / 2);

		glTexCoord2f(2.0 / 4.0, 1.0 / 3.0);
		glVertex3f(a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0/4.0, 1.0 / 3.0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();

	glRotatef(180, 1, 0, 0);
	glRotatef(180, 0, 0, 1);
	glBegin(GL_QUADS);
	{
		glTexCoord2f(1.0, 1.0 / 3.0);
		glVertex3f(-a / 2, a / 2, a / 2);

		glTexCoord2f(3.0 / 4.0, 1.0 / 3.0);
		glVertex3f(-a / 2, -a / 2, a / 2);

		glTexCoord2f(3.0 / 4.0, 2.0 / 3.0);
		glVertex3f(a / 2, -a / 2, a / 2);

		glTexCoord2f(1.0, 2.0 / 3.0);
		glVertex3f(a / 2, a / 2, a / 2);
	}
	glEnd();

	glPopMatrix();
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
}

void CGLRenderer::OnKeyDown(UINT nChar)
{
	if (nChar == VK_UP) {
		a1 += 10 * toRad;
	}
	if (nChar == VK_DOWN) {
		a1 -= 10 * toRad;
	}
	if (nChar == VK_LEFT) {
		a2 += 10 * toRad;
	}
	if (nChar == VK_RIGHT) {
		a2 -= 10 * toRad;
	}
	if (nChar == 'Q') {
		a3 += 10 * toRad;
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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexEnvf(GL_TEXTURE_2D, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img->Width(), img->Height(), GL_RGBA, GL_UNSIGNED_BYTE, img->GetDIBBits());
	
	glBindTexture(GL_TEXTURE_2D, 0);
	delete img;
	return texID;
}
