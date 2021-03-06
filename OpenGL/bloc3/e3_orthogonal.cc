#include "GLIncludes.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include "../Model/model.h"

#include "GameModel.h"
#include "GameObject.h"

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>

using namespace std;

int mx = 0, my = 0, edge = 600;
int rotX = 30, rotY = 30;
int traX = 0, traY = 0;
float scaleX = 1, scaleY = 1;
float xView = 0, yView = 0;

const char* WINDOW_NAME = "Hello World";

const string HOMER_MODEL_PATH = "../Model/HomerProves.obj";
const string RAYMAN_MODEL_PATH = "../objects/Rayman3/Rayman3.obj";
const string LEGOMAN_MODEL_PATH = "../Model/legoman.obj";
const string MICKEY_MODEL_PATH = "../objects/MickeyMouse/MickeyMouse.obj";
const string PATRICK_MODEL_PATH = "../Model/Patricio.obj";

int windowIndentifier;

const int SPACE_KEY = 32;
const int T_KEY = 116;
const int R_KEY = 114;
const int S_KEY = 115;
const int C_KEY = 99;
const int H_KEY = 104;
const int D_KEY = 100;
const int W_KEY = 119;
const int ESC_KEY = 27;
const int A_KEY = 97;
const int B_KEY = 98;

const int ROT_STATE = 0;
const int TRANS_STATE = 1;
const int SCALE_STATE = 2;
const int HERO_STATE = 3;
int currentState = 0;
vector<string> states;

bool DEBUG_MODE = false;
bool SPHERE_VISIBLE = true;
bool AXIS_VISIBLE = true;
bool BOX_VISIBLE = true;

float distPtoP (float p1x, float p1y, float p1z, float p2x, float p2y, float p2z) {
	return sqrt(pow(p2x-p1x,2)+pow(p2y-p1y,2)+pow(p2z-p1z,2));
}

float SCENE_HEIGHT = 1.2;
float SCENE_CENTER_Y = SCENE_HEIGHT/2.0 - 0.4;
float SCENE_RADIUS = distPtoP(0, SCENE_CENTER_Y, 0, -1.5, -0.4, -1.5);
float WINDOW_MIN_EDGE = SCENE_RADIUS;
float WME = WINDOW_MIN_EDGE; //for short

Model m;

void nextState () {
    currentState = ++currentState % states.size();
}

GameObject legoman;

void setColor (float r, float g, float b, float a) {
	glColor4f((float)r/0xFF, (float)g/0xFF, (float)b/0xFF, a);
}

void renderAxis (int size) {

	if (!AXIS_VISIBLE) return;

	glPushMatrix();

	glBegin(GL_LINES);

	glColor4f(1,0,0,1);
	glVertex3f(0,0,0);
	glVertex3f(size,0,0);

	glColor4f(0,1,0,1);
	glVertex3f(0,0,0);
	glVertex3f(0,size,0);
	
	glColor4f(0,0,1,1);
	glVertex3f(0,0,0);
	glVertex3f(0,0,size);

	glEnd();

	glPopMatrix();

}

//Snowman height = 2*0.4 + 2*0.2 = 0.8 + 0.4 = 1.2
void renderSnowman (void) {
	glPushMatrix();

	//glTranslated(0,-0.4,0);

	setColor(0xFF, 0xFF, 0xFF, 1);
	glutSolidSphere(0.4, 20, 20);

	setColor(0xFF, 0xFF, 0x00, 1);
	glTranslated(0, 0.6, 0);
	glutSolidSphere(0.2, 20, 20);

	glRotated(90, 0, 1, 0);
	glTranslated(0, 0, 0.15);

	setColor(0xF5, 0x87, 0x00, 1);
	glutSolidCone(0.1, 0.2, 20, 20);

	glPopMatrix();
}

void renderScene (void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();


	//Camera transformations
	//glTranslated(traX/500.0, traY/500.0, 0);
	glScaled(scaleX, scaleX, 1);
	//glTranslated(0,SCENE_CENTER_Y,0);
	glRotated(rotX/2.0, 0, 1, 0);
	glRotated(rotY/2.0, 1, 0, 0);

	//Stage axis render
	renderAxis(10);

	glTranslated(0,0,-0.5);
	glutWireSphere(0.25, 10, 10);
	/*
	glTranslated(0,-SCENE_CENTER_Y,0);

    //Hero render
   	legoman.render(AXIS_VISIBLE, BOX_VISIBLE);
	
	//Floor render
	setColor(0x37,0xA4,0x3E, 1);
	//setColor(rand()%256, rand()%256, rand()%256, 1);
	glBegin(GL_QUADS);
		glVertex3f(-1.5, -0.4, -1.5);
		glVertex3f(-1.5, -0.4, 1.5);
		glVertex3f(1.5, -0.4, 1.5);
		glVertex3f(1.5, -0.4, -1.5);
	glEnd();

	//Visual guide for -0.75,x,-0.75
	glTranslated(0.75,-0.4,0.75);
	renderAxis(1);
	glTranslated(-0.75,0.4,-0.75);

	renderSnowman();

	glTranslated(0, SCENE_CENTER_Y, 0);
	if (SPHERE_VISIBLE) glutWireSphere(SCENE_RADIUS, 50, 50);
	glTranslated(0, SCENE_HEIGHT/2.0, 0);
	renderAxis(1);
	*/

	glPopMatrix();
	glutSwapBuffers();
	

}

void reshapeWindow (int w, int h) {
	/*
	edge = std::min(w,h);
	xView = (w-edge)/2.0;
	yView = (h-edge)/2.0;
	glViewport(xView,yView,edge,edge);
	*/
	glViewport(0,0,w,h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ratio = (float) w/h;
	float quo = 1.0;
	if (ratio < 1) {
		quo /= ratio;
		ratio = 1.0;
	}
	glOrtho(-WME*ratio, WME*ratio, -WME*quo, WME*quo, -WME*quo, WME*quo);
	glMatrixMode(GL_MODELVIEW);
}

void mouseEvent (int button, int state, int x, int y) {
	mx = x;
	my = y;
}

void renderUI () {
	
	system("clear");
   	cout << endl << "-- ENTREGA2 VIEWER --" << endl << endl;

   	for (int i = 0; i < states.size(); ++i) {
   		cout << "[";
   		if (i == currentState) cout << "*";
   		else cout << " ";
   		cout << "] " << states[i] << endl;
   	}

   	cout << endl << "R:(" << rotX << "," << rotY << "), T:(" << traX << "," << traY << "), S:(" << scaleX << ")" << endl << endl;

   	cout << "(D) to enable key debug." << endl;
   	cout << "(A) to toggle axis." << endl;
   	cout << "(B) to toggle boxes." << endl;
   	cout << "(W) to toggle sphere container." << endl;
   	cout << "ESC to exit." << endl << endl;

}

void mouseMove (int x, int y) {

	switch(currentState) {

		case ROT_STATE:
			rotX += x - mx;
			rotY += y - my;
			break;

		case TRANS_STATE:
			traX += x - mx;
			traY += my - y;
			break;
		
		case SCALE_STATE:
			scaleX *= (x-mx)/100.0 + 1;
			scaleY *= (y-my)/100.0 + 1;
			break;

		case HERO_STATE:
			legoman.p[0] += (x-mx)/100.0;
			legoman.p[2] += (y-my)/100.0;
			break;

	}

	mx = x;
	my = y;
	glutPostRedisplay();
	renderUI();

}

void keyboardEvent (unsigned char key, int x, int y) {

    int oldState = currentState;

    if (DEBUG_MODE) cout << (int)key << endl;

    switch((int)key) {

        case SPACE_KEY: nextState();
            break;

        case T_KEY: currentState = TRANS_STATE;
            break;

        case R_KEY: currentState = ROT_STATE;
            break;

        case S_KEY: currentState = SCALE_STATE;
            break;

        case H_KEY: currentState = HERO_STATE;
        	break;

        case ESC_KEY: cout << "KTHXBYE" << endl << endl;
        	exit(0);
        	break;

        case D_KEY:
			DEBUG_MODE = !DEBUG_MODE;
			break;

		case W_KEY:
			SPHERE_VISIBLE = !SPHERE_VISIBLE;
			glutPostRedisplay();
			break;

		case A_KEY:
			AXIS_VISIBLE = !AXIS_VISIBLE;
			glutPostRedisplay();
			break;

		case B_KEY:
			BOX_VISIBLE = !BOX_VISIBLE;
			glutPostRedisplay();
			break;

    }

   if (oldState != currentState) {
   		renderUI();
   }

}

int main (int argc, const char * argv []) {

	srand (time(NULL));

	states.push_back("Rotate! (R)");
	states.push_back("Translate! (T)");
	states.push_back("Scale! (S)");
	states.push_back("Hero! (H)");
	nextState();

	GameModel legomanModel (PATRICK_MODEL_PATH);
	legoman = GameObject(legomanModel);

	legoman.p[1] = -0.4 + 0.5/2; //Feet position + half height

	legoman.p[0] = 0.75;
	legoman.p[1] = -0.4;
	legoman.p[2] = 0.75;

	legoman.s[0] = legoman.s[1] = legoman.s[2] = 0.5*0.5;

	legoman.regPoint[0] = 1;
	legoman.regPoint[1] = -1;
	legoman.regPoint[2] = 1;

	//legoman.

	glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(edge,edge);
	windowIndentifier = glutCreateWindow(WINDOW_NAME);
    glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	int size = 1;
	glOrtho(-size,size,-size,size,-size,size);
	glMatrixMode(GL_MODELVIEW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor((float)0xDA/0xFF, (float)0xC2/0xFF, (float)0xEB/0xFF, 1);

	glutReshapeFunc(reshapeWindow);
	glutDisplayFunc(renderScene);
	glutMouseFunc(mouseEvent);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboardEvent);

	renderUI();

	//glutFullScreen();

	glutMainLoop();

  return 0;
}
