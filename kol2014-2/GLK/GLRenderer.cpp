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

	tex = LoadTexture(CString("OpenGL.bmp"));

	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	x = 20 * cos(a1) * cos(a2);
	y = 20 * sin(a1);
	z = 20 * cos(a1) * sin(a2);
	gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);


	SetWhiteLight();

	glBindTexture(GL_TEXTURE_2D, tex);
	DrawRubikCube(2, 3);

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
	gluPerspective(40, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glDeleteTextures(1, &tex);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawRubikCube(double a, int count)
{
	glPushMatrix();
	float alpha = a * (count / 2) * 1.05;
	glTranslatef(-alpha, -alpha, -alpha);

	for (int i = 0; i < count; i++) {
		glPushMatrix();
		if (i < 3) {
			glTranslatef(0, alpha, alpha);
			glRotatef(rotK[i] * 180 / 3.14159, 1, 0, 0);
			glTranslatef(0, -alpha, -alpha);
		}
		for (int j = 0; j < count; j++) {
			glPushMatrix();

			for (int k = 0; k < count; k++) {
				DrawCube(a, k,j );
				glTranslatef(0, 0, a * 1.05);
			}
			glPopMatrix();

			glTranslatef(0, a * 1.05, 0);
		}
		glPopMatrix();
		glTranslatef(a * 1.05, 0, 0);
	}
	glPopMatrix();

}

void CGLRenderer::DrawASide(float a, float s, float t)
{
	s = (int)s % 3;
	t = (int)t % 3;
	float step = 1.0 / 3.0;
	glBegin(GL_QUADS);

	glNormal3f(0, 0, 1);

	glTexCoord2f(step * s, step * t);
	glVertex3f(-a / 2, a / 2, a / 2);
	glTexCoord2f(step * s, step * (t + 1));
	glVertex3f(-a / 2, -a / 2, a / 2);
	glTexCoord2f(step * (s + 1), step * (t + 1));
	glVertex3f(a / 2, -a / 2, a / 2);
	glTexCoord2f(step * (s+1), step);
	glVertex3f(a / 2, a / 2, a / 2);

	glEnd();
}

void CGLRenderer::DrawCube(float a, float s, float t)
{
	glPushMatrix();

	SetMaterial(1.0, 0, 0);
	DrawASide(a, s, t);

	glRotatef(90, 0, 1, 0);

	SetMaterial(0, 0, 1.0);
	DrawASide(a, ((int)s + 1) % 3, t);

	glRotatef(90, 0, 1, 0);

	SetMaterial(1, 0.50, 0.0);
	DrawASide(a, ((int)s + 2) % 3, t);

	glRotatef(90, 0, 1, 0);

	SetMaterial(0.0, 1.0, 0.0);
	DrawASide(a, ((int)s + 3) % 3, t);

	glRotatef(90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);

	SetMaterial(1.0, 1.0, 0.0);
	DrawASide(a, s, ((int)t + 1)%3);

	glRotatef(180, 1, 0, 0);

	SetMaterial(1.0, 1.0, 1.0);
	DrawASide(a, s, ((int)t - 1) % 3);

	glEnd();

	glPopMatrix();
}

void CGLRenderer::DrawAxis()
{
	glDisable(GL_LIGHTING);

	glBegin(GL_LINES);

	glColor3f(1.0, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);

	glColor3f(0, 1.0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);

	glColor3f(0, 0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);

	glEnd();

}

void CGLRenderer::OnKeyDown(UINT nChar)
{
	if (nChar == 'Q') {
		a2 += 0.05;
	}
	if (nChar == 'W') {
		a2 -= 0.05;
	}
	if (nChar == VK_UP) {
		a1 += 0.05;
	}
	if (nChar == VK_DOWN) {
		a1 -= 0.05;
	}
	if (nChar == '1') {
		rotK[0] += 0.05;
	}
	if (nChar == '2') {
		rotK[1] += 0.05;
	}
	if (nChar == '3') {
		rotK[2] += 0.05;
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
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img->Width(), img->Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img->GetDIBBits());
	
	glBindTexture(GL_TEXTURE_2D, 0);
	delete img;
	return texID;
}

void CGLRenderer::SetMaterial(float r, float g, float b)
{
	float ambient[] = {r/5, g/5, b/5};
	float difspec[] = { r, g, b };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, difspec);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, difspec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
}

void CGLRenderer::SetWhiteLight()
{
	float amb[] = {0.5, 0.5, 0.5, 1.0};
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	float dif[] = { 1.0, 1.0, 1.0, 1.0 };
	float amb0[] = { 0.8, 0.8, 0.8, 1.0 };
	float spec[] = { 0.5, 0.5, 0.5, 1.0 };
	float pos0[] = { 0.0, 1.0, 0.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, pos0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

	float pos1[] = { x, y, z, 1.0 };
	float str = sqrt(x * x + y * y + z * z);

	float dir[] = { -x/str, -y/str, -z/str};

	glLightfv(GL_LIGHT1, GL_POSITION, pos1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec);

	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 13);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2);

	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
}
