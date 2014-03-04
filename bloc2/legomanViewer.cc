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

bool rotTransToogle = false;

const int ROT_STATE = 0;
const int TRANS_STATE = 1;
const int SCALE_STATE = 2;
int currentState = 0;
vector<string> states;

Model m;

void nextState () {
    currentState = ++currentState % states.size();
}

vector<GameModel> gameModels;
vector<GameObject> gameObjects;

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

void renderModel (Model &model) {

    vector<Face> faces = model.faces();

    for (int i = 0; i < faces.size(); ++i) {

        const Face &f = faces[i];
        const Material &mat = Materials[f.mat];

        glBegin(GL_TRIANGLES);

        for (int j = 0; j < f.v.size(); ++j) {
            const float *d = mat.diffuse;
            glColor4f(d[0],d[1],d[2],d[3]);
            glVertex3dv(&model.vertices()[f.v[j]]);
        }

        glEnd();


    }

}

void renderScene (void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	renderAxis(2);
	glPushMatrix();

	glTranslated(traX/500.0, traY/500.0, 0);

	glScaled(scaleX, scaleX, 1);

	glRotated(rotX/2.0, 0, 1, 0);
	glRotated(rotY/2.0, 1, 0, 0);

    	renderAxis(1);

   	//renderModel(m);
	for (int i = 0; i < gameObjects.size(); ++i) gameObjects[i].render();
		
	glColor4f((float)0x7C/0xFF, (float)0xD2/0xFF, (float)0x79/0xFF, 1);
	glBegin(GL_QUADS);
		glVertex3f(-1.5, -0.4, -1.5);
		glVertex3f(-1.5, -0.4, 1.5);
		glVertex3f(1.5, -0.4, 1.5);
		glVertex3f(1.5, -0.4, -1.5);
	glEnd();

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

	cout << "R:(" << rotX << "," << rotY << "), T:(" << traX << "," << traY << "), S:(" << scaleX << ")" << endl;
}

void keyboardEvent (unsigned char key, int x, int y) {

    int oldState = currentState;

    switch((int)key) {

        case SPACE_KEY: nextState();
            break;

        case T_KEY: currentState = TRANS_STATE;
            break;

        case R_KEY: currentState = ROT_STATE;
            break;

        case S_KEY: currentState = SCALE_STATE;
            break;

    }

   if (oldState != currentState) cout << states[currentState] << endl << endl;

}

int main (int argc, const char * argv []) {

	system("clear");
	srand (time(NULL));

	states.push_back("Rotate!");
	states.push_back("Translate!");
	states.push_back("Scale!");
	nextState();

	vector <string> modelPaths;
	modelPaths.push_back(HOMER_MODEL_PATH);
	modelPaths.push_back(LEGOMAN_MODEL_PATH);

	for (int i = 0; i< modelPaths.size(); ++i) gameModels.push_back(GameModel(modelPaths[i]));
	int gameModelsSize = gameModels.size();

	for (int i = 0; i < 10; ++i) {

		GameObject legoman (gameModels[rand()%gameModelsSize]);

		legoman.p[0] = rand()%200/100.0 - 1;
		legoman.p[2] = rand()%200/100.0 - 1;

		legoman.s[0] = legoman.s[1] = legoman.s[2] = 0.4;

		legoman.r[1] = rand()%360;

		gameObjects.push_back(legoman);

	}

	glutInit(&argc, (char **)argv);
    	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(edge,edge);
	windowIndentifier = glutCreateWindow(WINDOW_NAME);
    	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1,1,-1,1,-1,1);
	glMatrixMode(GL_MODELVIEW);
    	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glClearColor(255, 255, 255, 1);

	glutReshapeFunc(reshapeWindow);
	glutDisplayFunc(renderScene);
	glutMouseFunc(mouseEvent);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboardEvent);
	glutMainLoop();

  return 0;
}
