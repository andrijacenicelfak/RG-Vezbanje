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
	glClearColor(0.4, 0.4, 1.0, 1.0);
	glEnable(GL_DEPTH_TEST);
	
	tex[0] = LoadTexture(CString("Eiffel-1a.bmp"));
	tex[1] = LoadTexture(CString("Eiffel-2a.bmp"));
	tex[2] = LoadTexture(CString("Eiffel-3a.bmp"));
	tex[3] = LoadTexture(CString("Eiffel-4a.bmp"));

	toranj = new float*[12];
	for (int i = 0; i < 12; i++)
		toranj[i] = new float[16*3];

	int i = 0;
	FillVA(toranj[i], 4.00, 2.40, 1.50); FillVANorm(toranj[i + 1], 4.00, 2.40, 1.50); FillVTex(toranj[i + 2], 0.215, 0.00);
	i += 3;
	FillVA(toranj[i], 2.40, 1.20, 1.50); FillVANorm(toranj[i + 1], 2.40, 1.20, 1.50); FillVTex(toranj[i + 2], 0.215, 0.00);
	i += 3;
	FillVA(toranj[i], 1.08, 0.00, 8.00); FillVANorm(toranj[i + 1], 1.08, 0.00, 8.00); FillVTex(toranj[i + 2], 0.450, 0.05);
	i += 3;
	FillVA(toranj[i], 2.40, 2.64, 0.25); FillVANorm(toranj[i + 1], 2.40, 2.64, 0.25); FillVTex(toranj[i + 2], 0.000, 0.00);

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

	//float x = 15 * cos(a1) * cos(a2);
	//float y = 15 * sin(a1);
	//float z = 15 * cos(a1) * sin(a2);

	gluLookAt(15, 5, 15, 0, 4, 0, 0, 1, 0);

	float pos[] = {10.0, 15.0, 10.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPushMatrix();
	glRotatef(a2 * 180 / 3.14159, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	DrawPart(0);

	glTranslatef(0, 1.5, 0);

	glBindTexture(GL_TEXTURE_2D, tex[3]);
	DrawPart(9);

	glBindTexture(GL_TEXTURE_2D, tex[1]);
	DrawPart(3);

	glTranslatef(0, 1.5, 0);

	glPushMatrix();
	glScalef(1.08/2.4, 1, 1.08 / 2.4);
	glBindTexture(GL_TEXTURE_2D, tex[3]);
	DrawPart(9);
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, tex[2]);
	DrawPart(6);
	glPopMatrix();

	//glBindTexture(GL_TEXTURE_2D, tex[3]);
	//glBegin(GL_QUADS);
	//glNormal3f(0, 1, 0);
	//glTexCoord2f(0, 0);
	//glVertex3f(-1, 0, -1);
	//glTexCoord2f(0, 1);
	//glVertex3f(-1, 0, 1);
	//glTexCoord2f(1, 1);
	//glVertex3f(1, 0, 1);
	//glTexCoord2f(1, 0);
	//glVertex3f(1, 0, -1);
	//glEnd();

	glDisable(GL_LIGHTING);
	//DrawAxis();

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
	gluPerspective(45, (double)w / (double)h, 0.1, 2000);
	glMatrixMode(GL_MODELVIEW);
	//---------------------------------
	wglMakeCurrent(NULL, NULL);
}

void CGLRenderer::DestroyScene(CDC *pDC)
{
	wglMakeCurrent(pDC->m_hDC, m_hrc);

	glDeleteTextures(4, tex);

	for (int i = 0; i < 12; i++)
		delete[] toranj[i];
	delete[] toranj;

	wglMakeCurrent(NULL,NULL); 
	if(m_hrc) 
	{
		wglDeleteContext(m_hrc);
		m_hrc = NULL;
	}
}

void CGLRenderer::DrawPart(int index)
{
	glBegin(GL_QUADS);
	for (int i = 0; i < 16; i++) {

		glTexCoord2f(toranj[index+ 2][i * 2], toranj[index+2][i * 2 + 1]);
		glNormal3f(toranj[index +1][i * 3], toranj[index+ 1][i * 3 + 1], toranj[index+1][i * 3 + 2]);
		glVertex3f(toranj[index][i * 3], toranj[index][i * 3 + 1], toranj[index][i * 3 + 2]);
	}
	glEnd();
}

void CGLRenderer::FillVA(float* buff, float a, float b, float h)
{
	int i = 0;
	buff[i] = -b / 2; buff[i+1] = h; buff[i+2] = -b / 2;
	i += 3;
	buff[i] = -a / 2; buff[i+1] = 0; buff[i+2] = -a / 2;

	i += 3;
	buff[i] = -a / 2; buff[i+1] = 0; buff[i+2] = a / 2;
	i += 3;
	buff[i] = -b / 2; buff[i+1] = h; buff[i+2] = b / 2;
	//
	i += 3;
	buff[i] = -b / 2; buff[i+1] = h; buff[i+2] = b / 2;
	i += 3;
	buff[i] = -a / 2; buff[i+1] = 0; buff[i+2] = a / 2;

	i += 3;
	buff[i] = a / 2; buff[i + 1] = 0; buff[i + 2] = a / 2;
	i += 3;
	buff[i] = b / 2; buff[i + 1] = h; buff[i + 2] = b / 2;
	//
	i += 3;
	buff[i] = b / 2; buff[i + 1] = h; buff[i + 2] = b / 2;
	i += 3;
	buff[i] = a / 2; buff[i + 1] = 0; buff[i + 2] = a / 2;
	
	i += 3;
	buff[i] = a / 2; buff[i + 1] = 0; buff[i + 2] = -a / 2;
	i += 3;
	buff[i] = b / 2; buff[i + 1] = h; buff[i + 2] = -b / 2;
	//
	i += 3;
	buff[i] = b / 2; buff[i + 1] = h; buff[i + 2] = -b / 2;
	i += 3;
	buff[i] = a / 2; buff[i + 1] = 0; buff[i + 2] = -a / 2;

	i += 3;
	buff[i] = -a / 2; buff[i + 1] = 0; buff[i + 2] = -a / 2;
	i += 3;
	buff[i] = -b / 2; buff[i + 1] = h; buff[i + 2] = -b / 2;


}

void CGLRenderer::FillVANorm(float* buff, float a, float b, float h)
{
	int i = 0;
	double alpha = atan(h / (a - b));
	double beta = -135 * 3.14159 / 180;
	for(int j = 0; j < 4; j++){
		buff[i] = sin(beta); buff[i + 1] = sin(alpha); buff[i + 2] = cos(beta);
		i += 3;
		buff[i] = sin(beta); buff[i + 1] = sin(alpha); buff[i + 2] = cos(beta);
		i += 3;
		beta += 3.14159 / 2;
		buff[i] = sin(beta); buff[i + 1] = sin(alpha); buff[i + 2] = cos(beta);
		i += 3;
		buff[i] = sin(beta); buff[i + 1] = sin(alpha); buff[i + 2] = cos(beta);
		i += 3;
	}
}

void CGLRenderer::FillVTex(float* buff, float x1, float x2)
{
	int i = 0;
	for (int j = 0; j < 4; j++) {
		buff[i] = x1; buff[i + 1] = 0.0;
		i += 2;
		buff[i] = x2; buff[i + 1] = 1;
		i += 2;
		buff[i] = 1 - x2; buff[i + 1] = 1;
		i += 2;
		buff[i] = 1 - x1; buff[i + 1] = 0.0;
		i += 2;
	}
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

	glColor3f(0, 0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);

	glEnd();
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
