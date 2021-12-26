#define _WCHAR_T_DEFINED
#define _STDCALL_SUPPORTED
/**** HEADER FILES ****/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <windows.h>
using namespace std;

/**** Global Declarations and initializations *****/

GLUquadricObj* quadratic; //draw a quadric

int arraySize = 11000; //number of snow flakes
float snow[11000][2];   //coordinates of snow flakes
int j;                  //snow[specifies flake number][0=specifies x coordinate  1=specifies y coordinate]
GLfloat angle = 0.0f; //the angle to rotate the snowman
GLfloat x, y, z; //snowman variables

float Light1, Light2, Light3; //the lights on the gifts on the tree
float r = 0.5, g = 0.5, b = 0.5; //the color of text
#define MAX_CHAR 128  //128 ASCII characters between 0 and 127 in total
#define MAX_PART 120 //number of particle
int first = 0; //flag to set off fireworks

static GLdouble axix = 0.0, axiz = 1.0;


/***** Initialize the snow ******/

void initSnow()
{
	int i;
	for (i = 0; i < arraySize; i++)
	{
		int r1 = rand() % 6000;
		int r2 = rand() % 1000;
		while (r2 < 50)
		{
			r2 = rand() % 1000;
		}
		snow[i][0] = r1 * -0.01;//x co-ordinate
		snow[i][1] = r2 * 0.01;//y co-ordinate
	}
}


/***** Reshape function ******/

void resize(int w, int h)
{
	GLdouble size;
	GLdouble aspect;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLfloat ar = (GLfloat)w / h;
	gluPerspective(45, ar, 0.1, 100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/***** Funtion to output text on screen *****/

void selectFont(int size, int charset, const char* face)
{
	//Font function in <windows.h>
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 1, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void drawString(const char* str)
{
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) // If this is the first call, perform initialization
	{
		// If this is the first call, perform initialization
		isFirstCall = 0;
		// Request MAX_CHAR for consecutive display list numbers
		lists = glGenLists(MAX_CHAR);
		// Install the drawing command for each character into the corresponding display list
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// Invoke the display list for each character and draw each character
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

/***** Function to Draw Snow ******/

void drawSnow()
{

	glColor3d(0.95, 0.95, 0.95); // Color of the snow
	srand(time(NULL));

	int i;
	for (i = 0; i < arraySize; i++)
	{
		glPushMatrix();
		glTranslated(snow[i][0], snow[i][1], -4);
		glRotated(65, -1.0, 0.0, 0.8);
		glutSolidCube(0.015);
		glPopMatrix();
		glPushMatrix();
		glTranslated(-snow[i][0], snow[i][1], -4);
		glRotated(65, -1.0, 0.0, 0.8);
		glutSolidCube(0.015);
		glPopMatrix();
	}
}



/****** Function to Draw Snowman *******/

void drawSnowMan(GLfloat x, GLfloat y, GLfloat z)
{
	glColor3f(0.0, 0.0, 0.0);
	glPushMatrix();        // Eyes
	glTranslatef(x, y + 0.6, z + 0.5);
	glutSolidSphere(0.05, 200, 200);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(x + 0.2, y + 0.6, z + 0.5);
	glutSolidSphere(0.05, 200, 200);
	glPopMatrix();

	glColor3d(1, 0, 0);  // Nose
	glPushMatrix();
	glTranslatef(x + 0.1, y + 0.45, z + 0.5);
	glRotatef(-260.0, 1.0, -1.5, 0.0);
	glutSolidCone(0.08, 0.2, 10, 2);
	glPopMatrix();

	glColor3f(1, 1, 1);  // Body Structure
	glPushMatrix();
	glTranslatef(x, y, z);
	glutSolidSphere(0.55, 200, 200);
	glPopMatrix();
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(x, y - 0.79, z);
	glutSolidSphere(0.95, 200, 200);
	glPopMatrix();
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef(x, y + 0.5, z);
	glutSolidSphere(0.35, 200, 200);
	glPopMatrix();

}


/***** Function to draw the rotated snowman ******/

void movableSnowMan()
{
	x = -2.7, y = 0.0, z = -7;
	glPushMatrix();
	glTranslatef(x, y, z);
	glRotatef(angle, 0.0, 0.0, 1.0);
	glTranslatef(-x, -y, -z);
	drawSnowMan(x, y, z);
	glPopMatrix();
}


/****** Function to Draw the Tree ******/

void drawTree()
{
	glColor3d(0.1, 0.5, 0.1);  // Tree color

	//Tree Cones
	glPushMatrix();
	glTranslated(0.0, 0.1, -6);
	glRotated(65, -1.0, 0.0, 0.0);
	glutSolidCone(0.85, 1.6, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, -0.2, -6);
	glRotated(65, -1.0, 0.0, 0.0);
	glutSolidCone(0.9, 1.5, 100, 100);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.0, -0.3, -6);
	glRotated(65, -1.0, 0.0, 0.0);
	glutSolidCone(0.95, 1.5, 100, 100);
	glPopMatrix();

	// Tree base
	glColor3d(0.29, 0.13, 0.13);
	glPushMatrix();
	glTranslated(0.0, -1.5, -6);
	glRotated(65, -1.0, 0.0, 0.0);
	gluCylinder(quadratic, 0.3f, 0.3f, 1.25f, 32, 32);
	glPopMatrix();

	// Tree shadow
	glColor3d(0.6, 0.6, 0.6);
	glPushMatrix();
	glTranslated(0.0, -1.2, -6);
	glRotated(70, -1.0, 0.0, 0.0);
	glutSolidTorus(0.33, 0.60, 200, 200);
	glPopMatrix();
}

/******* Funtion for Drawing lights on the tree ******/

void drawOrnament()
{

	// Eight Lights on the tree

	glColor3d(Light1 - 0.01, Light2, Light3);
	glPushMatrix();
	glTranslated(-0.3, 0.1, -5);
	glRotated(65, -1.0, 0.0, 0.8);
	glutSolidCube(0.1);
	glPopMatrix();

	glColor3d(Light2, Light3 + 0.06, Light1);
	glPushMatrix();
	glTranslated(0.3, 0.45, -5);
	glRotated(65, -1.0, 0.0, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

	glColor3d(Light1, Light3, Light2 - 0.05);
	glPushMatrix();
	glTranslated(0, 1.1, -5);
	glRotated(65, -1.0, 0.0, 0.5);
	glutSolidCube(0.1);
	glPopMatrix();

	glColor3d(Light2 - 0.04, Light1 + 0.02, Light3);
	glPushMatrix();
	glTranslated(0.4, -0.25, -5);
	glRotated(65, -1.0, 0.0, 0.8);
	glutSolidCube(0.1);
	glPopMatrix();

	glColor3d(Light3 + 0.02, Light1 - 0.08, Light2 - 0.06);
	glPushMatrix();
	glTranslated(-0.2, -0.45, -5);
	glRotated(65, -1.0, 0.0, 0.8);
	glutSolidCube(0.1);
	glPopMatrix();

	glColor3d(Light3 - 0.04, Light2 + 0.07, Light1 - 0.07);
	glPushMatrix();
	glTranslated(-0.5, 0.5, -5);
	glRotated(65, -1.0, 0.0, 0.8);
	glutSolidCube(0.1);
	glPopMatrix();

	glColor3d(Light3 + 0.06, Light1, Light1);
	glPushMatrix();
	glTranslated(-0.1, 0.7, -5);
	glRotated(65, -1.0, 0.0, 0.8);
	glutSolidCube(0.1);
	glPopMatrix();

	glColor3d(Light2, Light3 + 0.02, Light3 - 0.04);
	glPushMatrix();
	glTranslated(0.2, 0.05, -5);
	glRotated(65, -1.0, 0.0, 0.8);
	glutSolidCube(0.1);
	glPopMatrix();
}
/****** Function to Draw the ground/mountains ******/

void drawGround()
{
	//Upper Ground Circle
	glColor3f(0.93, 0.94, 0.94);
	glPushMatrix();
	glTranslatef(-7, -11, -12.5);
	glutSolidSphere(10, 200, 1000);
	glPopMatrix();

	//Middle Ground Circle
	glColor3f(0.96, 0.97, 0.95);
	glPushMatrix();
	glTranslatef(1.5, -11, -10.5);
	glutSolidSphere(10, 200, 1000);
	glPopMatrix();

	//Back Ground Circle
	glColor3f(0.91, 0.92, 0.90);
	glPushMatrix();
	glTranslatef(13, -11, -19.5);
	glutSolidSphere(10, 200, 1000);
	glPopMatrix();

}

void sunLight()
{
	GLfloat sun_light_position[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat sun_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);
}
void drawSun()
{

	GLfloat sun_mat_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat sun_mat_diffuse[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat sun_mat_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	GLfloat sun_mat_emission[] = { 0.3f, 0.2f, 0.2f, 1.0f };
	GLfloat sun_mat_shininess = 0.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, sun_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, sun_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, sun_mat_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, sun_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, sun_mat_shininess);
	glRotatef(0, 0.0f, -1.0f, 0.0f);
	glTranslatef(0.5f, 0.8f, 0.0f);
	glColor4d(0.92, 0.89, 0.41, 1.0);
	glutSolidSphere(0.1, 200, 200);

}
struct Particle
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat speed;
	GLfloat time;
	GLfloat x_offset;
	GLfloat y_offset;
	GLfloat z_offset;
};

Particle p[MAX_PART];

static GLfloat colors[12][3] =
{
	{1.0f,0.5f,0.5f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.5f},{0.75f,1.0f,0.5f},
	{0.5f,1.0f,0.5f},{0.5f,1.0f,0.75f},{0.5f,1.0f,1.0f},{0.5f,0.75f,1.0f},
	{0.5f,0.5f,1.0f},{0.75f,0.5f,1.0f},{1.0f,0.5f,1.0f},{1.0f,0.5f,0.75f}
};
void fireworkbox()
{
	glColor3f(0, 1, 1);
	glColor3f(1, 0, 1);
	glColor3f(1, 1, 0);
	glPushMatrix();
	glTranslatef(-2.8f, -1.2f, -3.5f);
	glutSolidCube(0.3f);
	glPopMatrix();
}
void firework()
{
	glTranslatef(-2.8f, -1.2f, -3.5f);
	if (first == 1)
	{
		for (int i = 0; i < MAX_PART; i++)
		{
			p[i].x = 0.0f;
			p[i].y = 0.0f;
			p[i].z = 0.0f;
			p[i].speed = 0.0001f;
			p[i].time = rand() % 10 + 1; // rand between 1-10
			p[i].x_offset = (double)(rand() % 1000) / 100000 - 0.005;
			p[i].y_offset = (double)(rand() % 1000) / 100000 + 0.001;
			p[i].z_offset = (double)(rand() % 1000) / 100000 - 0.005;
		}
		first = 0;
	}
	glPointSize(4.0);
	glBegin(GL_POINTS);
	for (int i = 0; i < MAX_PART; i++)
	{
		glColor3f(colors[i % 12][0], colors[i % 12][1], colors[i % 12][2]);
		glVertex3f(p[i].x, p[i].y, p[i].z);
		p[i].time -= 0.001;
		if (p[i].time <= 0.0)
		{
			p[i].x = 0.0f;
			p[i].y = 0.0f;
			p[i].z = 0.0f;
			p[i].time = rand() % 10 + 7; // rand between 7-17
		}
		p[i].x += (p[i].speed + p[i].x_offset);
		p[i].y += (p[i].speed + p[i].y_offset);
		p[i].z += (p[i].speed + p[i].z_offset);
	}
	glEnd();
}

/****** Init Function *******/
void init() {
	glClearColor(0.45, 0.45, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}
/****** Display Function *******/

void display(void)
{
	//set the view
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(axix, 0, axiz, 0, 0, 0, 0, 1, 0);
	//draw the text
	selectFont(48, ANSI_CHARSET, "Comic Sans MS");
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(r, g, b);
	glRasterPos2f(-0.5f, 0.25f);
	drawString("HAPPY CHRISTMAS!");

	glTranslatef(0.0, -0.5, 0.0);
	sunLight();
	drawSun();
	drawGround();
	movableSnowMan();
	drawSnow();
	drawTree();
	drawOrnament();
	fireworkbox();
	firework();
	glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);

	glutSwapBuffers();
}

/****** Keyboard Function to move the snowman *******/

void myKeyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
	case 'A':
		angle -= 0.9f;
		break;

	case 'd':
	case 'D':
		angle += 0.9f;
		break;
	}
	glutPostRedisplay();
}

/***** Mouse Function to set off the fireworks *****/

void myMouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		first = 1;
	}
	display();
}

/****** Idle Function which animates the scene ******/

void idle()
{
	srand(time(NULL));
	Light1 = ((float)(rand()) / (float)(RAND_MAX));	  // randomize
	Light2 = ((float)(rand()) / (float)(RAND_MAX)) * 0.99; // color of the
	Light3 = ((float)(rand()) / (float)(RAND_MAX));      // lights

	for (int j = 0; j < arraySize; j++)
	{
		snow[j][0] += 0.005;   // moving the snow in x direction
		snow[j][1] -= 0.005;   // moving the snow in y direction
	}
	r = r + 0.02; // change the color of the text
	g = g + 0.02;
	b = b + 0.02;
	glutPostRedisplay();
}


/**** Function to display the whole scene *****/

void displayScene()
{
	glutIdleFunc(idle);
	display();
	glFlush();
}

void myArrow_keys(int a_keys, int x, int y) {
	switch (a_keys) {
	case GLUT_KEY_LEFT:
		axix -= 0.1f;
		break;
	case GLUT_KEY_RIGHT:
		axix += 0.1f;
		break;
	case GLUT_KEY_UP:
		axiz += 0.1f;
		break;
	case GLUT_KEY_DOWN:
		axiz -= 0.1f;
		break;
	default:
		break;
	}
}
/******* MAIN FUNCTION *******/

int main(int argc, char* argv[])
{

	// Description of the project.
	cout << "This is the project for SWE206 Computer Graphics." << endl;
	cout << "Done by DMT1909215 ZHAI XINGXIANG & SWE1909475 LEI YITAO." << endl;
	cout << "In the program, you can use 'A' and 'D' to rotate the snowman." << endl;
	cout << "You can use arrow keys to change the view." << endl;
	cout << "And you can click left mouse button to set off the fireworks." << endl;
	cout << "Please press any button to enter the program." << endl;
	system("pause");

	// Initializations
	glutInit(&argc, argv);
	glutInitWindowSize(800, 500); // window size
	glutInitWindowPosition(50, 40); // window position
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH);
	glutCreateWindow("Happy Christmas");
	init();
	quadratic = gluNewQuadric();

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST); //enabling z-buffer

	// Function Calls
	glutReshapeFunc(resize);
	glutDisplayFunc(displayScene);
	glDepthFunc(GL_LESS);
	glutKeyboardFunc(myKeyboardFunc);
	glutMouseFunc(myMouseFunc);
	glutSpecialFunc(myArrow_keys);
	initSnow();

	glutMainLoop(); // Initiate the main loop
	return 0;
}

