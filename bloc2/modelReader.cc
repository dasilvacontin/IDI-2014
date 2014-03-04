#if defined(__APPLE__)
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
  #include <GLUT/glut.h>
  #include <OpenGL/OpenGL.h>
  #include <GLUT/GLUT.h>
#else
  #include <GL/gl.h>
  #include <GL/freeglut.h>
#endif

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include "model.h"
using namespace std;

int mx = 0, my = 0, edge = 600;
int rotX = 0, rotY = 0;
int traX = 0, traY = 0;
float scaleX = 1, scaleY = 1;
float xView = 0, yView = 0;

const char* WINDOW_NAME = "Hello World";
string MODEL_PATH = "aibsbiaius";
int windowIndentifier;

int SPACE_KEY = 32;
bool rotTransToogle = false;

const int ROT_STATE = 0;
const int TRANS_STATE = 1;
const int SCALE_STATE = 2;
int currentState = 0;
vector<string> states;

Model m;

void nextState () {
	currentState = ++currentState % states.size();
	cout << states[currentState] << endl << endl;
}

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

void renderScene (void) {

	glClear(GL_COLOR_BUFFER_BIT);
	renderAxis(2);
	glPushMatrix();

	glTranslated(traX/500.0, traY/500.0, 0);

	glScaled(scaleX, scaleX, 1);

	glRotated(rotX/2.0, 0, 1, 0);
	glRotated(rotY/2.0, 1, 0, 0);

	vector<Face> faces = m.faces();

	for (int i = 0; i < faces.size(); ++i) {

		Face f = faces[i];
		vector<vector<double>> v = f.vertices();

		glBegin(GL_TRIANGLES);

		for (int j = 0; i < vertices.size(); ++i) {
			glVertex3f(v[j][0],v[j][1],v[j][2]);
		}

		glEnd();


	}

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
	}

	mx = x;
	my = y;
	glutPostRedisplay();
}

void keyboardEvent (unsigned char key, int x, int y) {

	if ((int)key == SPACE_KEY) nextState();

}

int main (int argc, const char * argv []) {

	system("clear");

	states.push_back("Rotate!");
	states.push_back("Translate!");
	states.push_back("Scale!");
	nextState();

	m.load(MODEL_PATH);

	glutInit(&argc, (char **)argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(edge,edge);
	windowIndentifier = glutCreateWindow(WINDOW_NAME);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);

	glutReshapeFunc(reshapeWindow);
	glutDisplayFunc(renderScene);
	glutMouseFunc(mouseEvent);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboardEvent);
	glutMainLoop();

  return 0;
}
