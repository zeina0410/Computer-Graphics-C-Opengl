#include <GL/glut.h>
#include <Math.H>
#include <mmsystem.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int angle = 0;

double xt = 0;
double ex = 0;
double ez = 0;
bool lo = true;

double HI = 0;
bool m = true;

double HI2 = 1;
bool m2 = true;
GLUquadricObj* a = gluNewQuadric();

int tex[8];

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case VK_ESCAPE:
		exit(0);
		break;
	//To Turn Light On and Off
	case 'l':
		if (lo == true)
			lo = false;
		else 
			lo = true;
		break;
	default:
		break;
	}
}

void Special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_RIGHT:
		xt += 0.1;
		break;
	case GLUT_KEY_LEFT:
		xt -= 0.1;
		break;
	case GLUT_KEY_UP:
		ex = ex + cos(xt);
		ez = ez + sin(xt);
		break;
	case GLUT_KEY_DOWN:
		ex = ex - cos(xt);
		ez = ez - sin(xt);
		break;
	default:
		break;
	}
}

GLuint LoadTexture(const char imagename[]) {
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping/filtering options (on the currently bound texture object)

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//GL_ADD, GL_MODULATE, GL_DECAL, GL_BLEND, GL_REPLACE.

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(imagename, &width, &height, &nrChannels, 0);

	if (data) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	}
	else {
		MessageBox(NULL, L"Failed to load texture", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
	}

	stbi_image_free(data);
	return textureID;
}

GLuint LoadTexture1(const char imagename[]) { //only for png image extension
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping/filtering options (on the currently bound texture object)

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	//GL_ADD, GL_MODULATE, GL_DECAL, GL_BLEND, GL_REPLACE.

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(imagename, &width, &height, &nrChannels, 0);

	if (data) {
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

	}
	else {
		MessageBox(NULL, L"Failed to load texture", L"SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
	}

	stbi_image_free(data);
	return textureID;
}

void InitGL(void) {
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.15f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	//Skybox
	glEnable(GL_TEXTURE_2D);
	tex[0] = LoadTexture("Recources/glacier_dn.tga");
	tex[1] = LoadTexture("Recources/glacier_up.tga");
	tex[2] = LoadTexture("Recources/glacier_rt.tga");
	tex[3] = LoadTexture("Recources/glacier_bk.tga");
	tex[4] = LoadTexture("Recources/glacier_lf.tga");
	tex[5] = LoadTexture("Recources/glacier_ft.tga");

	//Texture
	tex[6] = LoadTexture("Recources/venus.bmp");
	tex[7] = LoadTexture("Recources/posy.jpg");

	//Sound Effects
	PlaySound(L"Recources/park.wav", 0, SND_ASYNC | SND_LOOP);

	//Lighting
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_LIGHTING);

	//Light Source for Bulbs
	float Di[] = { 1,1,1 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Di);
	float Sp[] = { 1,1,1 };
	glLightfv(GL_LIGHT1, GL_SPECULAR, Sp);

	//Light Bulbs
	glNewList(1, GL_COMPILE);
	glEnable(GL_LIGHT1);
	glTranslatef(0, -4, 0);
	glPushMatrix();
	glColor3f(0.08, 0.06, 0.04);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 0.5, 0.5, 16, 30, 30);
	glColor3f(1, 1, 0.9);
	glutSolidSphere(2.5, 30, 30);
	glPopMatrix();
	glDisable(GL_LIGHT1);
	glEndList();

	//Trees
	glNewList(2, GL_COMPILE);
	glPushMatrix();
	glColor3f(0, 0.5, 0);
	glRotated(-90, 1, 0, 0);
	glutSolidCone(1, 3, 30, 30);
	glColor3f(0, 0.6, 0);
	glTranslated(0, 0, 1);
	glutSolidCone(1, 3, 30, 30);
	glPopMatrix();
	glPushMatrix();
	glColor3f(0.42, 0.38, 0.36);
	glRotated(90, 1, 0, 0);
	gluCylinder(a, 0.3, 0.3, 3, 30, 30);
	glPopMatrix();
	glEndList();

	//Bench
	glNewList(4, GL_COMPILE);
	glPushMatrix();
	glColor3f(1, 1, 1);
	glTranslatef(1.5, 0, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 0.2, 0.2, 4, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-1.5, 0, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 0.2, 0.2, 4, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(1.5, -2, 1);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 0.2, 0.2, 2, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-1.5, -2, 1);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 0.2, 0.2, 2, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glColor3f(0.3, 0.25, 0.21);
	glTranslatef(0, -0.8, 0.1);
	glScalef(1, 0.35, 0.1);
	glutSolidCube(4);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, -2.2, 0.5);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 0.35, 0.1);
	glutSolidCube(4);
	glPopMatrix();
	glEndList();

	//Tree With Beches Around
	glNewList(5, GL_COMPILE);
	glPushMatrix();

	glTranslatef(0, -6, 0);
	glScalef(5, 5, 5);
	glCallList(2);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, -10.8, 0);
	glScalef(2.5, 2.5, 2.5);
	glPushMatrix();
	/* PUSH NEW MATRIX */
	glTranslatef(2.8, 0, 0);
	glRotatef(90, 0, 1, 0);
	glCallList(4);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-2.8, 0, 0);
	glRotatef(-90, 0, 1, 0);
	glCallList(4);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, 0, 2.8);
	glCallList(4);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, 0, -2.8);
	glRotatef(180, 0, 1, 0);
	glCallList(4);
	glPopMatrix();
	/* POP NEW MATRIX */
	glPopMatrix();
	glEndList();

	//Chair and Tables
	glNewList(3, GL_COMPILE);
	glScalef(0.3, 0.3, 0.3);
	glTranslatef(0, 0, -70);
	glPushMatrix();
	glColor3f(0.2, 0.15, 0.11);
	glTranslatef(0, -5, -61);
	glEnable(GL_CLIP_PLANE0);
	double c3[] = { 0,1,0,0 };
	glClipPlane(GL_CLIP_PLANE0, c3);
	glEnable(GL_CLIP_PLANE1);
	double d3[] = { 0,0,-1,-6.5 };
	glClipPlane(GL_CLIP_PLANE1, d3);
	glutSolidCube(15);
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, -17, -65);
	glEnable(GL_CLIP_PLANE0);
	double c2[] = { 0,1,0,0 };
	glClipPlane(GL_CLIP_PLANE0, c2);
	glEnable(GL_CLIP_PLANE1);
	double d2[] = { 0,1,0,-7 };
	glClipPlane(GL_CLIP_PLANE1, d2);
	glutSolidCube(15);
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, -10, -65);
	glRotatef(90, 1, 0, 0);
	glScalef(0.5, 0.5, 1);
	glColor3f(0, 0, 0);
	gluCylinder(a, 1, 1, 9, 32, 16);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(5, -10, -65);
	glRotatef(90, 1, 0, 0);
	glScalef(0.5, 0.5, 1);
	gluCylinder(a, 1, 1, 9, 32, 16);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, 0, -70);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1, 1);
	gluCylinder(a, 1, 1, 20, 80, 4);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(5, 0, -70);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1, 1);
	gluCylinder(a, 1, 1, 20, 80, 4);
	glPopMatrix();
	glPushMatrix();

	glColor3f(0.2, 0.15, 0.11);
	glTranslatef(0, -17, -35);
	glEnable(GL_CLIP_PLANE0);
	double c1[] = { 0,1,0,0 };
	glClipPlane(GL_CLIP_PLANE0, c1);
	glEnable(GL_CLIP_PLANE1);
	double d1[] = { 0,1,0,-7 };
	glClipPlane(GL_CLIP_PLANE1, d1);
	glutSolidCube(15);
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, -5, -25);
	glEnable(GL_CLIP_PLANE0);
	double c[] = { 0,1,0,0 };
	glClipPlane(GL_CLIP_PLANE0, c);
	glEnable(GL_CLIP_PLANE1);
	double d[] = { 0,0,-1,-6.5 };
	glClipPlane(GL_CLIP_PLANE1, d);
	glutSolidCube(15);
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(5, -10, -35);
	glRotatef(90, 1, 0, 0);
	glScalef(0.5, 0.5, 1);
	glColor3f(0, 0, 0);
	gluCylinder(a, 1, 1, 9, 32, 16);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, -10, -35);
	glRotatef(90, 1, 0, 0);
	glScalef(0.5, 0.5, 1);
	gluCylinder(a, 1, 1, 9, 32, 16);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, 0, -30);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1, 1);
	gluCylinder(a, 1, 1, 20, 80, 4);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(5, 0, -30);
	glRotatef(90, 1, 0, 0);
	glScalef(1, 1, 1);
	gluCylinder(a, 1, 1, 20, 80, 4);
	glPopMatrix();

	glTranslatef(0, -20, -50);
	glPushMatrix();

	glScalef(10.0, 1.0, 10.0);
	glTranslatef(0, 10, 0);
	glColor3f(0.15, 0.08, 0.06);
	glutSolidCube(2);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(9, 0, 9);
	glRotatef(-90, 1, 0, 0);
	glColor3f(0, 0, 0);
	gluCylinder(a, 1, 1, 9, 32, 16);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(9, 0, -9);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(a, 1, 1, 9, 32, 16);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-9, 0, -9);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(a, 1, 1, 9, 32, 16);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-9, 0, 9);
	glRotatef(-90, 1, 0, 0);
	gluCylinder(a, 1, 1, 9, 32, 16);
	glPopMatrix();

	glEndList();

	// Bars For Game 1
	glNewList(10, GL_COMPILE);
	glPushMatrix();
	glRotated(90, 1, 1, 0);
	gluCylinder(a, 0.5, 0.5, 20, 10, 10); //moving bars
	glutSolidCube(2); //upper meet point
	glPopMatrix();
	glPushMatrix();

	glTranslated(16, -15, 0);
	glutSolidTeapot(3); //setting place
	glPopMatrix();
	glEndList();

	//1 Fence
	glNewList(11, GL_COMPILE);
	glPushMatrix();
	glColor3f(0.08, 0.06, 0.04);
	glTranslated(0, -2, 0);
	glRotated(-90, 1, 0, 0);
	gluCylinder(a, 0.5, 0.5, 5, 10, 10);
	glPopMatrix();
	glPushMatrix();

	glTranslated(3, 2, 0);
	glRotated(-90, 0, 1, 0);
	gluCylinder(a, 0.5, 0.5, 5, 10, 10);
	glPopMatrix();
	glEndList();

	//Fence List
	glNewList(12, GL_COMPILE);
	glPushMatrix();
	for (int i = -42; i < 42; i+=3) {
		for (int j = -42; j <= 42; j += 84) {
			glPushMatrix();
			glTranslatef(i, 0, j);
			glCallList(11);
			glPopMatrix();
		}
	}
	glPopMatrix();
	glPushMatrix();

	for (int i = -42; i <= 42; i += 3) {
		glPushMatrix();
		glTranslatef(42, 0, i);
		glRotatef(90, 0, 1, 0);
		glCallList(11);
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();

	for (int i = -42; i <= -9; i += 3) {
		glPushMatrix();
		glTranslatef(-42, 0, i);
		glRotatef(90, 0, 1, 0);
		glCallList(11);
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();

	for (int i = 9; i <= 42; i += 3) {
		glPushMatrix();
		glTranslatef(-42, 0, i);
		glRotatef(90, 0, 1, 0);
		glCallList(11);
		glPopMatrix();
	}
	glPopMatrix();
	glEndList();

	//Bars for Ferris Wheel
	glNewList(13, GL_COMPILE);
	glPushMatrix();
	glColor3f(0, 1, 1);
	glTranslatef(0, 10, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 0.5, 0.5, 20, 30, 30);
	glPopMatrix();
	glEndList();

	//Carts for Ferris Wheel
	glNewList(14, GL_COMPILE);
	glPushMatrix();
	glTranslatef(10, 9, 0);
	glutSolidSphere(3, 30, 30);
	glPopMatrix();
	glEndList();

	//Teacup
	glNewList(15, GL_COMPILE); //list number 12 
	glPushMatrix();
	glTranslated(0, -10, 0);
	glRotated(-90, 1, 0, 0);
	gluCylinder(a, 7, 6, 10, 15, 15);
	glPopMatrix();
	glPushMatrix();

	glTranslated(0, 0.2, 0);
	glRotated(-90, 1, 0, 0);
	glColor3ub(75, 50, 20);
	gluCylinder(a, 6, 7, 5, 15, 15);
	glPopMatrix();
	glEndList();

	//Bench for Up-Down
	glNewList(16, GL_COMPILE);
	glPushMatrix();
	glColor3f(1, 0.3, 0);
	glRotated(90, 0, 1, 0); //set
	glTranslated(0, -2, -4);
	glScaled(2, 0.1, 0.5);
	glutSolidCube(10);
	glPopMatrix();
	glPushMatrix();

	glRotated(90, 0, 1, 0); //backset
	glTranslated(0, 0, -2);
	glRotated(90, 1, 0, 0);
	glScaled(2, 0.1, 0.5);
	glutSolidCube(10);
	glPopMatrix();
	glPushMatrix();

	glColor3f(0, 1, 1);
	glTranslatef(-5, 0.5, -10);
	gluCylinder(a, 0.3, 0.3, 20, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-5, 0.4, -10);
	glRotatef(90, 0, 1, 0);
	glRotatef(45, 1, 0, 0);
	gluCylinder(a, 0.3, 0.3, 4, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-5, 0.4, 10);
	glRotatef(90, 0, 1, 0);
	glRotatef(45, 1, 0, 0);
	gluCylinder(a, 0.3, 0.3, 4, 30, 30);
	glPopMatrix();
	glEndList();

	//Car
	glNewList(17, GL_COMPILE);
	glPushMatrix();
	glColor3f(0, 0.3, 0.5);
	glScalef(2, 0.8, 1);
	glutSolidCube(4);
	glTranslatef(0, 3, 0);
	glColor3f(0, 0.8, 1);
	glutSolidCube(3);
	glPopMatrix();
	glPushMatrix();

	glColor3f(0.5, 0.5, 0.5);
	glTranslated(-3, -1.5, -2.5);
	gluDisk(a, 0, 1, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glTranslated(-3, -1.5, 2.5);
	gluDisk(a, 0, 1, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glTranslated(3, -1.5, -2.5);
	gluDisk(a, 0, 1, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glTranslated(3, -1.5, 2.5);
	gluDisk(a, 0, 1, 30, 30);
	glPopMatrix();
	glEndList();

	//Bars For Game 7
	glNewList(18, GL_COMPILE); //H_CY
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0, 30, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 0.5, 0.5, 29, 50, 50);
	glPopMatrix();
	glEndList();

	//Ticket Booth
	glNewList(19, GL_COMPILE); //H_CY
	glPushMatrix();
	glScalef(0.4, 0.6, 0.4);
	glPushMatrix();
	glColor3f(0.2, 0, 0.5);
	glTranslatef(9, 0, 0);
	glScalef(0.2, 0.4, 1);
	glutSolidCube(27);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-9, 0, 0);
	glScalef(0.2, 0.4, 1);
	glutSolidCube(27);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, 0, 10.8);
	glScalef(0.5, 0.4, 0.2);
	glutSolidCube(27);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, 0, -10.8);
	glScalef(0.5, 0.4, 0.2);
	glutSolidCube(27);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, 18, 0);
	glScalef(1, 0.2, 1);
	glutSolidCube(27);
	glPopMatrix();
	glPushMatrix();

	glColor3f(1, 1, 1);
	glTranslatef(9, 17, 10.8);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 2, 2, 12, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(9, 17, -10.8);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 2, 2, 12, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-9, 17, 10.8);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 2, 2, 12, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-9, 17, -10.8);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 2, 2, 12, 30, 30);
	glPopMatrix();
	glPopMatrix();
	glEndList();
}

void SkyBoxCube() {
	float P1[] = { 1, -1,  1 };
	float P2[] = { 1, -1, -1 };
	float P3[] = { -1,-1, -1 };
	float P4[] = { -1,-1,  1 };
	float P5[] = { 1, 1,  1 };
	float P6[] = { 1, 1, -1 };
	float P7[] = { -1, 1, -1 };
	float P8[] = { -1, 1,  1 };
	glColor3f(1, 1, 1);
	//bottom
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 0); glVertex3fv(P1);
	glTexCoord2f(0, 0); glVertex3fv(P2);
	glTexCoord2f(0, 1); glVertex3fv(P3);
	glTexCoord2f(1, 1); glVertex3fv(P4);
	glEnd();
	//top
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3fv(P6);
	glTexCoord2f(1, 1); glVertex3fv(P5);
	glTexCoord2f(1, 0); glVertex3fv(P8);
	glTexCoord2f(0, 0); glVertex3fv(P7);
	glEnd();
	//right
	glBindTexture(GL_TEXTURE_2D, tex[2]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1); glVertex3fv(P1);//1
	glTexCoord2f(1, 0); glVertex3fv(P5);//2
	glTexCoord2f(0, 0); glVertex3fv(P6);//6
	glTexCoord2f(0, 1); glVertex3fv(P2);//5
	glEnd();
	//front
	glBindTexture(GL_TEXTURE_2D, tex[3]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1); glVertex3fv(P2);//2
	glTexCoord2f(1, 0); glVertex3fv(P6);//3
	glTexCoord2f(0, 0); glVertex3fv(P7);//7
	glTexCoord2f(0, 1); glVertex3fv(P3);//6
	glEnd();
	//left
	glBindTexture(GL_TEXTURE_2D, tex[4]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1); glVertex3fv(P3);//3
	glTexCoord2f(1, 0); glVertex3fv(P7);//4
	glTexCoord2f(0, 0); glVertex3fv(P8);//8
	glTexCoord2f(0, 1); glVertex3fv(P4);//7
	glEnd();
	//back
	glBindTexture(GL_TEXTURE_2D, tex[5]);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1); glVertex3fv(P4);//4
	glTexCoord2f(1, 0); glVertex3fv(P8);//1
	glTexCoord2f(0, 0); glVertex3fv(P5);//5
	glTexCoord2f(0, 1); glVertex3fv(P1);//8
	glEnd();
}

void SkyBoxFunc() {
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	SkyBoxCube();
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}

void ReSizeGLScene(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void DrawGLScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//Skybox
	glPushMatrix();
	glRotatef(xt * 180 / 3.14, 0, 1, 0);
	SkyBoxFunc();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	//Camera Movement
	/* PUSH MAIN MATRIX */
	gluLookAt(ex, 0, ez, ex + cos(xt), 0, ez + sin(xt), 0, 1, 0);
	glTranslatef(70, 0, -15);
	glScalef(0.3, 0.3, 0.3);
	glRotatef(-90, 0, 1, 0);
	glPushMatrix();

	//Main Light Source
	float Po[] = { 0,50,0,1 };
	glLightfv(GL_LIGHT0, GL_POSITION, Po);

	//Main Ground
	glColor3f(0.4, 0.4, 0.4);
	glBegin(GL_QUADS);
	glVertex3d(190, -20, -190);
	glVertex3d(-190, -20, -190);
	glVertex3d(-190, -20, 190);
	glVertex3d(190, -20, 190);
	glEnd();
	glPopMatrix();
	glPushMatrix();

	//Lake
	glTranslatef(-25, -19, 135);
	glRotatef(-90, -1, 0, 0);
	glScalef(2, 2, 1);
	glColor3f(0, 3, 6);//SHINE
	gluDisk(a, 0, 13.85, 49.85, 10);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-25, -18, 135);
	glRotatef(-90, -1, 0, 0);
	glScalef(2, 2, 1);
	glColor3f(0, 0, 0);//BROWN
	gluCylinder(a, 13.9, 13.9, 2, 30, 30);
	glPopMatrix();
	glPushMatrix();
	
	glTranslatef(-25, -15, 135);	//Teapot
	glRotated(angle, 0, 1, 0);
	glBindTexture(GL_TEXTURE_2D, tex[7]);
	glutSolidTeapot(6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	glPushMatrix();

	//Ticket Booth
	glTranslatef(-10, -16, -100);
	glScalef(1, 1.2, 1);
	glCallList(19);
	glPopMatrix();
	glPushMatrix();
	
	glTranslatef(25, -17, 160);
	glScalef(1, 1.2, 1.5);
	glCallList(19);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(75, -17, 160);
	glScalef(1, 1.2, 1.5);
	glCallList(19);
	glPopMatrix();
	glPushMatrix();

	//Main Fence
	glTranslatef(0, -16, 0);
	glPushMatrix();
	for (int i = -188; i <= 188; i += 3) {
		for (int j = -188; j <= 188; j += 376) {
			glPushMatrix();
			glTranslatef(j, 0, i); 
			glScalef(2, 2, 2);
			glRotatef(90, 0, 1, 0);
			glCallList(11);
			glPopMatrix();
		}
	}
	glPopMatrix();
	glPushMatrix();
	for (int i = -188; i <= 188; i += 3) {
		glPushMatrix();
		glTranslatef(i, 0, -188);
		glScalef(2, 2, 2);
		glCallList(11);
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();
	for (int i = -188; i <= 30; i += 3) {
		glPushMatrix();
		glTranslatef(i, 0, 188);
		glScalef(2, 2, 2);
		glCallList(11);
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();
	for (int i = 70; i <= 188; i += 3) {
		glPushMatrix();
		glTranslatef(i, 0, 188);
		glScalef(2, 2, 2);
		glCallList(11);
		glPopMatrix();
	}
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();

	//Distributing Lights
	for (int i = -90; i < 180; i += 90) {
		for (int j = -90; j < 180; j += 90) {
			glPushMatrix();
			glTranslatef(i, 0, j);
			glCallList(1);
			glPopMatrix();
		}
	}
	glPopMatrix();
	glPushMatrix();

	//Trees
	for (int i = -180; i < 180; i += 30) {
		for (int j = -180; j <= 180; j += 360) {
			glPushMatrix();
			glTranslatef(j, -5, i);
			glScalef(5, 5, 5);
			glCallList(2);
			glPopMatrix();
		}
	}
	glPopMatrix();
	glPushMatrix();
	for (int i = -180; i < 180; i += 30) {
		glPushMatrix();
		glTranslatef(i, -5, -180);
		glScalef(5, 5, 5);
		glCallList(2);
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();
	for (int i = 30; i >= -180; i -= 30) {
		glPushMatrix();
		glTranslatef(i, -5, 180);
		glScalef(5, 5, 5);
		glCallList(2);
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();
	for (int i = 70; i < 180; i += 30) {
		glPushMatrix();
		glTranslatef(i, -5, 180);
		glScalef(5, 5, 5);
		glCallList(2);
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();

	//Cafe
	/* PUSH NEW MATRIX 1 */
	glTranslatef(135, 0, 135);
	glPushMatrix();

	glColor3f(0.2, 0.15, 0.11);
	glTranslatef(0, -19, 0);
	glScalef(3, 0.2, 3);
	glutSolidCube(25);
	glPopMatrix();
	glPushMatrix();

	glColor3f(0.33, 0.25, 0.21);
	glTranslatef(0, 10, 0);
	glScalef(3, 0.1, 3);
	glutSolidCube(25);
	glPopMatrix();
	glPushMatrix();
	
	glColor3f(0.5, 0.5, 0.3);
	glTranslatef(36, -4, 0);
	glScalef(0.1, 1, 3);
	glutSolidCube(25);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, -4, 36);
	glScalef(3, 1, 0.1);
	glutSolidCube(25);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, -4, -36);
	glScalef(3, 1, 0.1);
	glutSolidCube(25);
	glPopMatrix();
	glPushMatrix();

	glColor3f(0, 0, 0);
	glTranslatef(24, -15, 0);
	glScalef(0.2, 0.35, 3);
	glutSolidCube(24);
	glPopMatrix();
	glPushMatrix();

	//Chairs and Tables
	for (int i = -30; i <= 20; i += 20) {
		for (int j = 15; j <= 60; j += 20) {
			glPushMatrix();
			glTranslatef(i, -10, j);
			glCallList(3);
			glPopMatrix();
		}

	}

	/* POP NEW MATRIX 1 */
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();

	//Benches and Tree
	glTranslatef(-45, 0, -45);
	glCallList(5);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(45, 0, -45);
	glCallList(5);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(-45, 0, 45);
	glCallList(5);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(45, 0, 45);
	glCallList(5);
	glPopMatrix();
	glPushMatrix();


	//Bar With Swings Rotating (Game 1)
	glTranslatef(135, -12, 45);
	glScalef(1.5, 1.5, 1.5);
	glPushMatrix();
	glTranslated(0, -5, 0);
	glRotated(-90, 1, 0, 0); //base bar
	glColor3f(2, 2, 0);
	gluCylinder(a, 2, 2, 23, 15, 15);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, 18, 0);
	glRotatef(angle, 0, 1, 0); //moving   you can use HI var  in case of pressing w  s 
	for (int i = 0; i <= 360; i += 72) {
		glPushMatrix();
		glRotatef(i++, 0, 1, 0);
		glColor3ub(i * 15, i * 12, i * 5);
		glCallList(10);
		glPopMatrix();
	}
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();

	//Fence for Game 1
	glTranslatef(135, -18, 45);
	glCallList(12);
	glPopMatrix();
	glPushMatrix();

	//Ferris Wheel (Game 2)
	glTranslatef(50, 30, -135);
	glScalef(3, 3, 3);
	glPushMatrix();
	//circle in the middle
	glColor3f(0, 1, 1);
	gluSphere(a, 2, 15, 15);
	glPopMatrix();
	glPushMatrix();

	//legs
	glColor3f(1, 1, 1);
	glRotatef(70, 1, 0, 0);
	glRotatef(-25, 0, 1, 0);
	gluCylinder(a, 1, 1, 20, 80, 4);
	glPopMatrix();
	glPushMatrix();

	glRotatef(70, 1, 0, 0);
	glRotatef(25, 0, 1, 0);
	gluCylinder(a, 1, 1, 20, 80, 4);
	glPopMatrix();
	glPushMatrix();

	glRotatef(110, 1, 0, 0);
	glRotatef(-25, 0, 1, 0);
	gluCylinder(a, 1, 1, 20, 80, 4);
	glPopMatrix();
	glPushMatrix();

	glRotatef(110, 1, 0, 0);
	glRotatef(25, 0, 1, 0);
	gluCylinder(a, 1, 1, 20, 80, 4);
	glPopMatrix();
	glPushMatrix();

	//moving matrix
	glRotatef(angle, 0, 0, 1);
	glPushMatrix();

	//main torus
	glColor3f(1, 1, 1);
	glutSolidTorus(1, 11, 30, 30);
	glPopMatrix();
	glPushMatrix();

	//inner d3amat
	for (int i = 0; i < 360; i += 45) {
		glPushMatrix();
		glRotatef(i, 0, 0, 1);
		glCallList(13);
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();

	//carts
	for (int i = 1; i < 361; i += 45) {
		glPushMatrix();
		glRotatef(i, 0, 0, 1);
		glColor3ub(i * 15, i * 12, i * 5);
		glCallList(14);
		glPopMatrix();
	}
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();

	//Fence for Game 2
	glTranslatef(45, -18, -135);
	glRotatef(90, 0, 1, 0);
	glCallList(12);
	glPopMatrix();
	glPushMatrix();

	//Disco (Game 3)
	glTranslatef(135, -10, -135);
	glScalef(2, 2, 2);
	glRotatef(angle++, 0, -1, 0);
	glPushMatrix();

	glRotatef(-80, -1, 0, 0);
	glColor3f(4, 2, 0);
	gluDisk(a, 0, 14, 50, 10);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, 1.9, -0.2);
	glRotatef(-80, -1, 0, 0);
	glColor3f(0, 0, 0);
	gluCylinder(a, 13.9, 13.9, 2, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glRotatef(-90, -1, 0, 0);
	glColor3f(0, 0, 0);
	gluCylinder(a, 0.75, 0.75, 5, 20, 20);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, -3, 0);
	glRotatef(90, 1, 0, 0);
	glColor3f(0, 0, 0);
	gluCylinder(a, 0, 2, 2, 20, 20);
	glPopMatrix();
	glPushMatrix();

	//Chair	
	glTranslatef(0, 1, -0.2);
	glRotatef(-80, -1, 0, 0);
	glColor3f(1, 0, 0);
	gluCylinder(a, 12.7, 12.7, 1, 30, 30);
	glPopMatrix();
	glPushMatrix();

	//upper
	glTranslatef(0, 1, -0.2);
	glRotatef(-80, -1, 0, 0);
	glColor3f(1, 0, 0);
	gluDisk(a, 12.5, 13.7, 50, 10);
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();

	//Fence for Game 3
	glTranslatef(135, -18, -135);
	glRotatef(90, 0, 1, 0);
	glCallList(12);
	glPopMatrix();
	glPushMatrix();

	//Teapot And Cups (Game 4)
	glTranslatef(-135, -10, 135);
	glScalef(0.5, 0.5, 0.5);
	glPushMatrix();
	glColor3f(0, 0, 0);
	glTranslatef(0, -10, 0);
	glRotated(90, 1, 0, 0);
	gluCylinder(a, 60, 60, 4, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glTranslatef(0, -10, 0);
	glRotated(90, 1, 0, 0);
	gluDisk(a, 0, 60, 30, 30);
	glPopMatrix();
	glPushMatrix();

	glRotated(angle, 0, 1, 0);
	glColor3ub(75, 75, 20);
	glBindTexture(GL_TEXTURE_2D, tex[6]);
	glutSolidTeapot(15);
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
	glPushMatrix();
	
	glRotated(angle, 0, 1, 0);
	for (float i = 1; i < 6.28; i += 0.628) {
		glPushMatrix();
		glTranslatef(50 * cos(i), 0, 50 * sin(i));
		glColor3ub(i * 25, i * 8, i * 20);
		glCallList(15);
		glPopMatrix();
	}
	glPopMatrix();
	glPopMatrix();
	glPushMatrix();

	//Fence for Game 4
	glTranslatef(-135, -18, 135);
	glRotatef(180, 0, 1, 0);
	glCallList(12);
	glPopMatrix();
	glPushMatrix();

	//Scisors (Game 5)
	glTranslatef(-135, -10, -45);
	glScalef(1.4, 1.4, 1.4);
	glRotatef(90, 0, 1, 0);
	glPushMatrix();
	glColor3ub(20, 70, 20); //centarball
	glTranslated(0, 30, 0);
	glutSolidSphere(5, 25, 25);
	glPopMatrix();
	glPushMatrix();

	glTranslated(-3, 30, 0);
	glRotated(-90, 0, 1, 0);   //hand --
	glColor3ub(100, 70, 0);
	gluCylinder(a, 2, 2, 5, 20, 20);
	glPopMatrix();
	glPushMatrix();

	glTranslated(8, 30, 0);
	glRotated(-90, 0, 1, 0);   //hand ++
	glColor3ub(100, 70, 0);
	gluCylinder(a, 2, 2, 5, 20, 20);
	glPopMatrix();
	glPushMatrix();

	glTranslated(0, -10, 0);
	glRotated(-90, 1, 0, 0); //bar
	glColor3ub(50, 0, 20);
	gluCylinder(a, 6, 3, 35, 35, 35);
	glPopMatrix();
	glPushMatrix();

	glColor3f(1, 1, 0);
	glRotatef(HI, 0, 0, 1);
	glTranslated(0, 40, 12);
	glRotated(110, 1, 0, 0);
	gluCylinder(a, 2, 2, 40, 20, 20);
	glutSolidCube(8);
	glPopMatrix();
	glPushMatrix();

	glColor3f(1, 0.3, 0);
	glRotatef(-HI, 0, 0, 1);
	glTranslated(0, 40, 12);
	glRotated(110, 1, 0, 0);
	gluCylinder(a, 2, 2, 40, 20, 20);
	glutSolidCube(8);
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();

	//Fence For Game 5
	glTranslatef(-135, -18, -45);
	glRotatef(180, 0, 1, 0);
	glCallList(12);
	glPopMatrix();
	glPushMatrix();

	//Up-Down (Game 6)
	glTranslatef(-135, -15, 45);
	glScalef(2, 2, 2);
	glRotatef(180, 0, 1, 0);
	glPushMatrix();
	glTranslated(0, -2, 0);   //base bar
	glColor3f(0, 0.7, 1);
	glRotated(-90, 1, 0, 0);
	gluCylinder(a, 2, 2, 35, 15, 15);
	glPopMatrix();
	glPushMatrix();

	glColor3f(1, 0.5, 0);
	glTranslatef(-13, 24, 0);
	glScalef(1.3, 1.3, 1.3);
	glCallList(14);
	glPopMatrix();
	glPushMatrix();

	glTranslated(0, HI2, 0);
	glCallList(16);
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();

	//Fence For Game 5
	glTranslatef(-135, -18, 45);
	glRotatef(180, 0, 1, 0);
	glCallList(12);
	glPopMatrix();
	glPushMatrix();

	//Merry Go Round (Game 7)
	glTranslatef(135, -12, -45);
	glPushMatrix();
	glColor3f(1, 0.8, 0);
	glTranslatef(0, -5, 0);
	glRotatef(90, -1, 0, 0);
	gluDisk(a, 0, 30, 30, 30);
	glPopMatrix();
	glPushMatrix();

	//Tours_Base
	glColor3f(1, 1, 1);
	glTranslatef(0, 2, 0);
	glRotatef(90, 0, -1, 0);
	glTranslatef(0, -8, 0);
	glRotatef(90, -1, 0, 0);
	glutSolidTorus(2, 30, 15, 15);
	glPopMatrix();
	glPushMatrix();

	glColor3f(1, 0.8, 0);
	glTranslatef(0, 35, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 1, 30, 15, 50, 50);
	glPopMatrix();
	glPushMatrix();

	//Torus
	glColor3f(1, 1, 1);
	glTranslatef(0, 20, 0);
	glRotatef(90, 0, -1, 0);
	glTranslatef(0, -1, 0);
	glRotatef(90, -1, 0, 0);
	glutSolidTorus(2, 30, 15, 15);
	glPopMatrix();
	glPushMatrix();

	//H_CY_Central
	glColor3f(0, 0, 0);
	glTranslatef(0, 26, 0);
	glRotatef(90, 1, 0, 0);
	gluCylinder(a, 1, 1, 32, 50, 50);
	glPopMatrix();
	glPushMatrix();

	glTranslated(0, -6, 0);
	glRotatef(90, 0, -1, 0);
	glRotatef(angle, 0, 1, 0);
	for (float j = 0; j < 6.28; j += 0.628) {
		glPushMatrix();
		glTranslatef(21 * cos(j), 0, 21 * sin(j));
		glCallList(18);
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();

	//Tours_Cy
	glTranslatef(0, 25, 0);
	glRotatef(90, 0, -1, 0);
	glTranslatef(0, -1, 0);
	glRotatef(90, -1, 0, 0);
	glutSolidTorus(1, 20, 15, 15);
	glPopMatrix();
	glPushMatrix();

	glTranslated(0, -2, 0);
	glRotatef(90, 0, -1, 0);
	glRotatef(angle, 0, 1, 0);
	for (float i = 0; i < 6.28; i += 0.628) {
		glPushMatrix();
		glTranslatef(20 * cos(i), 0, 20 * sin(i));
		glCallList(17);
		glPopMatrix();
	}
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();

	//Tent
	glTranslatef(-85, -17, -135);
	glScalef(1, 0.5, 0.5);
	glPushMatrix();
	glTranslated(0, -2, 0);
	glRotated(-90, 1, 0, 0);
	glColor3ub(0.2, 1, 0.5);
	gluCylinder(a, 80, 70, 40, 10, 10);
	glPopMatrix();
	glPushMatrix();

	glTranslated(0, 40, 0);
	glRotated(-90, 1, 0, 0);
	glColor3ub(75, 50, 20);
	gluCylinder(a, 70, 2, 50, 60, 60);
	glPopMatrix();

	/* POP MAIN MATRIX */
	glPopMatrix();
	glFlush();
}

void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / 60, timer, 0);

	angle++;

	//To Turn Light On and Off
	if (lo == true) {
		glEnable(GL_LIGHT0);
	}
	if (lo == false) {
		glDisable(GL_LIGHT0);
	}

	//To Move Game 5
	for (int i = -60; i <= 60; i++) {
		if (m == true) {
			HI += 0.015;
			if (HI >= 60) {
				m = false;
			}
		}
		if (m == false) {
			HI -= 0.015;
			if (HI <= -60) {
				m = true;
			}
		}
	}

	//To Move Game 6
	for (int i = -2; i <= 29; i++) {
		if (m2 == true) {
			HI2 += 0.02;
			if (HI2 >= 29) {
				m2 = false;
			}
		}
		if (m2 == false) {
			HI2 -= 0.02;
			if (HI2 <= 0) {
				m2 = true;
			}
		}
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE); // Use single display buffer.
	glutInitWindowSize(1080, 720);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("EMPTY WINDOW");
	InitGL();
	glutReshapeFunc(ReSizeGLScene);
	glutDisplayFunc(DrawGLScene);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(Special);
	glutTimerFunc(1000 / 60, timer, 0);
	glutMainLoop();
	return 0;
}