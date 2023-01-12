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
	char nazivS[] = "S0.jpg";
	char nazivM[] = "M0.jpg";
	char nazivTSC[] = "TSC0.jpg";
	for (int i = 0; i < 6; i++) {
		nazivS[1] = '0' + i;
		texS[i] = LoadTexture(CString(nazivS));
		nazivM[1] = '0' + i;
		texM[i] = LoadTexture(CString(nazivM));
		nazivTSC[3] = '0' + i;
		texTSC[i] = LoadTexture(CString(nazivTSC));
	}

	glEnable(GL_TEXTURE_2D);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DrawScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);
	//---------------------------------
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	float x = dist * cos(a1) * cos(a2);
	float y = dist * sin(a1);
	float z = dist * cos(a1) * sin(a2);

	gluLookAt(x, y, z, 0, 0, 0, 0, 1, 0);
	
	glPushMatrix();

	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glTranslatef(x, y, z);
	DrawSpace(1, 10);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();

	GLfloat pos[] = { 0.0, 0.0, 1.0, 0.0 };
	GLfloat pram[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, pram);
	glLightfv(GL_LIGHT0, GL_SPECULAR, pram);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, pram);
	

	if (svetlo) {
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}

	DrawEarth(3, 10);

	glPushMatrix();
	glTranslatef(-22, 0, -22);
	DrawMoon(1, 10);
	glPopMatrix();

	glDisable(GL_LIGHTING);
	DrawAxis();

	glPopMatrix();

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

	// delete textures
	glDeleteTextures(6, texM);
	glDeleteTextures(6, texS);
	glDeleteTextures(6, texTSC);

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawPatch(double R, int n)
{
	double step = 2.0 / (double)n;
	float x = -1.0, y = 1.0;
	float fi = 0;
	float o = 0;

	while(y > -1 + step) {
		glBegin(GL_TRIANGLE_STRIP);
		glColor3f(1, 1, 1);
		x = -1;
		while(x <= 1) {
			fi = atan(x);
			o = atan(y * cos(fi));
			float x2 = R * cos(o) * sin(fi);
			float y2 = R * sin(o);
			float z = R * cos(o) * cos(fi);

			glNormal3f(x2 / R, y2 / R, z / R);
			glTexCoord2f((x + 1.0) / 2.0, (-y + 1.0) / 2.0);
			glVertex3f(x2, y2, z);

			fi = atan(x);
			o = atan((y-step) * cos(fi));
			x2 = R * cos(o) * sin(fi);
			y2 = R * sin(o);
			z = R * cos(o) * cos(fi);

			glNormal3f(x2 / R, y2 / R, z / R);
			glTexCoord2f((x + 1.0) / 2.0, (-y+step + 1.0) / 2.0);
			glVertex3f(x2, y2, z);

			x += step;
		}
		glEnd();
		y -= step;
	}
}

void CGLRenderer::OnKeyDown(UINT nChar)
{
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
	if (nChar == 109) {
		dist += 0.5;
	}
	if (nChar == 107) {
		dist -= 0.5;
	}
	if (nChar == 'Q') {
		a3 += 0.05;
	}
	if (nChar == 'W') {
		a3 -= 0.05;
	}
	if (nChar == 'S') {
		svetlo = !svetlo;
	}
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

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, img->Width(), img->Height(), GL_BGRA_EXT, GL_UNSIGNED_BYTE, img->GetDIBBits());

	delete img;
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

void CGLRenderer::DrawSphere(double R, int n, UINT* tex)
{
	glPushMatrix();

	glBindTexture(GL_TEXTURE_2D, tex[0]);
	DrawPatch(R, n);

	glRotatef(90, 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, tex[1]);
	DrawPatch(R, n);

	glRotatef(90, 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, tex[2]);
	DrawPatch(R, n);

	glRotatef(90, 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, tex[3]);
	DrawPatch(R, n);

	glPopMatrix();

	glPushMatrix();

	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, tex[4]);
	DrawPatch(R, n);

	glRotatef(-180, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, tex[5]);
	DrawPatch(R, n);

	glPopMatrix();

}

void CGLRenderer::DrawEarth(double R, int tes)
{
	DrawSphere(R, tes, texTSC);
}

void CGLRenderer::DrawMoon(double R, int tes)
{
	glRotatef(a3 * 180 / 3.14159, 0, 1, 0);
	DrawSphere(R, tes, texM);
}

void CGLRenderer::DrawSpace(double R, int tes)
{
	DrawSphere(R,tes, texS);
}
