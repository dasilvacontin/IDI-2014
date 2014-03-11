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

int windowIndentifier;

const int SPACE_KEY = 32;
const int T_KEY = 116;
const int R_KEY = 114;
const int S_KEY = 115;
const int C_KEY = 99;
const int H_KEY = 104;
const int ESC_KEY = 27;

const int ROT_STATE = 0;
const int TRANS_STATE = 1;
const int SCALE_STATE = 2;
const int HERO_STATE = 3;
int currentState = 0;
vector<string> states;

Model m;

void nextState () {
    currentState = ++currentState % states.size();
}

GameObject legoman;

void renderAxis (int size) {

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

void renderSnowman (void) {
	glPushMatrix();

	glTranslated(0,-0.4,0);

	glScaled(scaleX, scaleX, 1);

	glRotated(rotX/2.0, 0, 1, 0);
	glRotated(rotY/2.0, 1, 0, 0);

	glutWireSphere(0.4, 10, 10);

	glTranslated(0, 0.6, 0);
	glutWireSphere(0.2, 10, 10);

	glRotated(90, 0, 1, 0);
	glTranslated(0, 0, 0.15);

	glutWireCone(0.1, 0.2, 10, 10);

	glPopMatrix();
}

void renderScene (void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();

	//Camera transformations
	glTranslated(traX/500.0, traY/500.0, 0);
	glScaled(scaleX, scaleX, 1);
	glRotated(rotX/2.0, 0, 1, 0);
	glRotated(rotY/2.0, 1, 0, 0);

	//Stage axis render
	renderAxis(1);

    //Hero render
   	legoman.render();
	
	//Floor render
	glColor4f((float)0x7C/0xFF, (float)0xD2/0xFF, (float)0x79/0xFF, 1);
	glBegin(GL_QUADS);
		glVertex3f(-1.5, -0.4, -1.5);
		glVertex3f(-1.5, -0.4, 1.5);
		glVertex3f(1.5, -0.4, 1.5);
		glVertex3f(1.5, -0.4, -1.5);
	glEnd();

	glBegin(GL_LINES);
		//Visual guide for -0.75,x,-0.75
		glColor4f(0,1,0,1);
		glVertex3f(0.75,-0.4,0.75);
		glVertex3f(0.75,0.4,0.75);
	glEnd();

	renderSnowman();

	glPopMatrix();
	glutSwapBuffers();

}

void reshapeWindow (int w, int h) {
	edge = std::min(w,h);
	xView = (w-edge)/2.0;
	yView = (h-edge)/2.0;
	glViewport(xView,yView,edge,edge);
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

    //cout << (int)key << endl;

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

	GameModel legomanModel (LEGOMAN_MODEL_PATH);
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
	glClearColor(255, 255, 255, 1);

	glutReshapeFunc(reshapeWindow);
	glutDisplayFunc(renderScene);
	glutMouseFunc(mouseEvent);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboardEvent);

	renderUI();

	glutMainLoop();

  return 0;
}
