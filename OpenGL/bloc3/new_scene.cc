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

#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

int d = 0;
int mx = 0, my = 0, edge = 600;
int rotX = 0, rotY = 0;
int traX = 0, traY = 0;
float zoom = 1;
float xView = 0, yView = 0;
float dist = 10;
float speed = 1;
float angle = -speed*2*M_PI*rotX/360;

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
const int W_KEY = 119;
const int ESC_KEY = 27;
const int A_KEY = 97;
const int B_KEY = 98;
const int C_KEY = 99;
const int D_KEY = 100;
const int E_KEY = 101;
const int F_KEY = 102;
const int G_KEY = 103;
const int H_KEY = 104;
const int I_KEY = 105;
const int O_KEY = 111;
const int P_KEY = 112;
const int Q_KEY = 113;
const int V_KEY = 118;

const int INSPECT_STATE = 0;
const int OPTIONS_STATE = 1;
const int FIRST_PERSON_STATE = 2;
int currentState = 0;
vector<string> states;

bool DEBUG_MODE = false;
bool SPHERE_VISIBLE = true;
bool AXIS_VISIBLE = true;
bool WALLS_VISIBLE = true;
bool BOX_VISIBLE = false;
bool ANIMATE = false;

bool ORTHO_CAMERA = true;
bool THIRD_CAMERA = false;

//FIRST_PERSON
vector <int> DOWN (256, 0);

float distPtoP (float p1x, float p1y, float p1z, float p2x, float p2y, float p2z) {
	return sqrt(pow(p2x-p1x,2)+pow(p2y-p1y,2)+pow(p2z-p1z,2));
}

vector<vector<Vertex> > floorVertex;
vector<vector<Vertex> > wallVertex;
vector<Vertex> VRP (3, 0);
vector<Vertex> EYE (3, 0);

//Mouse
vector<int> MB_STATE (5,1);


float viewportRatio;
float quo;
float vw;

float MIN_Y = -0.4;
float MAX_Y = MIN_Y + 1.2;
float SCENE_RADIUS = 0;
float WINDOW_MIN_EDGE = SCENE_RADIUS;
float WME = WINDOW_MIN_EDGE; //for short
float floor_size = 10;

Model m;
vector<Vertex> _box (3, 0);

vector <GameObject> gameobjects;

void randomVRP () {

	VRP[0] = rand()%(int)(floor_size+1) - floor_size/2;
	VRP[1] = 0;
	VRP[2] = rand()%(int)(floor_size+1) - floor_size/2;

}

void calculateSceneProperties () {

	SCENE_RADIUS = 0;

	for (int i = 0; i < floorVertex.size(); ++i) {
    	float ppdist = distPtoP(VRP[0], VRP[1], VRP[2], floorVertex[i][0], floorVertex[i][1], floorVertex[i][2]);
    	if (SCENE_RADIUS < ppdist) SCENE_RADIUS = ppdist;
    }

    if (WALLS_VISIBLE) {
    	for (int i = 0; i < wallVertex.size(); ++i) {
	    	float ppdist = distPtoP(VRP[0], VRP[1], VRP[2], wallVertex[i][0], wallVertex[i][1], wallVertex[i][2]);
	    	if (SCENE_RADIUS < ppdist) SCENE_RADIUS = ppdist;
	    }
    }

    WME = WINDOW_MIN_EDGE = SCENE_RADIUS;

}



void nextState () {
    currentState = ++currentState % states.size();
}



void setColor (float r, float g, float b, float a) {
	glColor4f((float)r/0xFF, (float)g/0xFF, (float)b/0xFF, a);
}

void recursiveBoxLine(std::vector<int> mod) {

	if (mod.size() < 3) {

		mod.push_back(-1);
		recursiveBoxLine(mod);
		mod.pop_back();
		mod.push_back(1);
		recursiveBoxLine(mod);

	} else {

		glBegin(GL_LINES);

		glColor4f(1,1,1,1);

		for (int i = 0; i < 3; ++i) {

			glVertex3f(_box[0]*mod[0]/2.0, _box[1]*mod[1]/2.0, _box[2]*mod[2]/2.0);

			int xmod = (i == 0)? -1 : 1;
			int ymod = (i == 1)? -1 : 1;
			int zmod = (i == 2)? -1 : 1;

			glVertex3f(xmod*_box[0]*mod[0]/2.0, ymod*_box[1]*mod[1]/2.0, zmod*_box[2]*mod[2]/2.0);

		}

		glEnd();

	}

}

void renderBox () {

	std::vector<int> mod (0);
	recursiveBoxLine(mod);

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
void renderSnowman () {
	glPushMatrix();

	glTranslated(0,0.4,0);

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

void renderUI () {
	
	system("clear");
   	cout << endl << "-- BLOC3 - EXERCICI1 --" << endl << endl;

   	for (int i = 0; i < states.size(); ++i) {
   		cout << "[";
   		if (i == currentState) cout << "*";
   		else cout << " ";
   		cout << "] " << states[i] << endl;
   	}

   	cout << endl << "R:(" << rotX << "," << rotY << ")" << endl;
   	cout << "VRP (" << VRP[0] << ", " << VRP[1] << ", " << VRP[2] << ")" << endl;
   	cout << "Zoom: " << zoom << endl;
   	cout << endl;

   	switch (currentState) {

   		case INSPECT_STATE:
   			cout << "Drag to rotate camera." << endl;
   			cout << "RightClick + Drag to zoom." << endl;
   			cout << endl;
   			cout << "(C) to toggle camera." << endl;
   			cout << "(R) to reset camera properties." << endl;
   			cout << "(SPACE) to randomize VRP." << endl;
   			cout << endl;
   			break;

   		case OPTIONS_STATE:
   			cout << "(A) to toggle Axis." << endl;
   			cout << "(B) to toggle Boxes." << endl;
   			cout << "(D) to enable key Debug." << endl;
   			cout << "(F) to toggle Fullscreen mode." << endl;
   			cout << "(V) to toggle walls." << endl;
   			cout << "(W) to toggle sphere Wireframe." << endl << endl;
   			break;

   		case FIRST_PERSON_STATE:
   			cout << "Drag to rotate." << endl;
   			cout << "[W] to move forward." << endl;
   			cout << endl;
   			break;

   	}
  
   	cout << "(ESC) to exit." << endl << endl;

}

void renderScene (void) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	//Stage axis render
	glTranslated(VRP[0], VRP[1], VRP[2]);
	//glRotated(-rotX/2.0, 0, 1, 0);
	//glRotated(-rotY/2.0, 1, 0, 0);
	setColor(0xaa, 0xaa, 0xaa, 1);
	if (SPHERE_VISIBLE) glutWireSphere(SCENE_RADIUS, 50, 50);
	renderAxis(SCENE_RADIUS);

	_box[0] = WME*vw*2;
	_box[1] = _box[2] = WME*quo*2;
	if (BOX_VISIBLE) renderBox();

	//glRotated(rotY/2.0, 1, 0, 0);
	//glRotated(rotX/2.0, 0, 1, 0);
	glTranslated(-VRP[0], -VRP[1], -VRP[2]);
	
    //Hero render
    for (int i = 0; i < gameobjects.size(); ++i) gameobjects[i].render(AXIS_VISIBLE, BOX_VISIBLE);
	
	//Floor render
	setColor(0x37,0xA4,0x3E, 1);
	//setColor(rand()%256, rand()%256, rand()%256, 1);
	glBegin(GL_QUADS);
		for (int i = 0; i < floorVertex.size(); ++i) glVertex3f(floorVertex[i][0], floorVertex[i][1], floorVertex[i][2]);
	glEnd();

	glPushMatrix();
	if (ANIMATE) glTranslated(0, -0.2 + 0.2*sin(d/10.0),0);

	glPushMatrix();
	glTranslated(2.5, 0, 2.5);
	renderSnowman();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2.5, 0, 2.5);
	renderSnowman();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-2.5, 0, -2.5);
	renderSnowman();
	glPopMatrix();

	glPopMatrix();


	//WALLS

	if (WALLS_VISIBLE) {

		setColor(0x33, 0x33, 0x33, 1);

		glPushMatrix();
		glTranslated(2.5, 1.5/2., -1.5);
		glScaled(4, 1.5, 0.2);
		glutSolidCube(1);
		glPopMatrix();

		glPushMatrix();
		glTranslated(-4.9,1.5/2.0,0);
		glScaled(0.2,1.5,10);
		glutSolidCube(1);
		glPopMatrix();

	}

	glPopMatrix();
	glutSwapBuffers();
	

}

int t = 0;

void setCamera () {

	renderUI();

	calculateSceneProperties();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (currentState == FIRST_PERSON_STATE) {

		gluPerspective(60, viewportRatio, 0.1, 1000);

	} else {

		quo = 1.0;
		vw = viewportRatio;
		if (viewportRatio < 1) {
			quo /= viewportRatio;
			vw = 1.0;
		}

		if (ORTHO_CAMERA) glOrtho(-WME*vw*zoom, WME*vw*zoom, -WME*quo*zoom, WME*quo*zoom, -WME*quo+dist, WME*quo+dist);
		else {

			float fovy = 2*atan(WME*quo/(dist-WME*quo))*(180/M_PI)*zoom;
			if (fovy > 180) fovy = 180;
			gluPerspective(fovy, viewportRatio, dist-WME*quo, dist+WME*quo);

		}

	}
	

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	if (currentState != FIRST_PERSON_STATE) {
	
		glTranslated(0,0,-dist);
		glRotated(0, 0, 0, 1);
		glRotated(rotY/2.0, 1, 0, 0);
		glRotated(rotX/2.0, 0, 1, 0);
		glTranslated(-VRP[0], -VRP[1], -VRP[2]);

	} else {

		angle = -speed*2*M_PI*rotX/360;
		GameObject &patrick = gameobjects[0];
		patrick.r[1] = angle*(360/(2*M_PI));
		cout << patrick.r[1] << endl;

		vector<Vertex> old_VRP (VRP);
		vector<Vertex> old_EYE (EYE);

		VRP[0] = patrick.p[0]+5*sin(angle);
		VRP[1] = patrick.p[1];
		VRP[2] = patrick.p[2]+5*cos(angle);



		if (THIRD_CAMERA) {
			EYE[0] = patrick.p[0]-5*sin(angle);
			EYE[1] = patrick.p[1]+2;
			EYE[2] = patrick.p[2]-5*cos(angle);
		} else {
			EYE[0] = patrick.p[0];
			EYE[1] = patrick.p[1]+1;
			EYE[2] = patrick.p[2];
		}

		if (ANIMATE) {
	
			EYE[0] = old_EYE[0] + (EYE[0]-old_EYE[0])/5.;
			EYE[1] = old_EYE[1] + (EYE[1]-old_EYE[1])/5.;
			EYE[2] = old_EYE[2] + (EYE[2]-old_EYE[2])/5.;

		}


		gluLookAt(	EYE[0], EYE[1], EYE[2],
					VRP[0],	VRP[1],	VRP[2],
					0,		1,		0);

	}

}

void reshapeWindow (int w, int h) {

	glViewport(0,0,w,h);
	viewportRatio = (float) w/h;

	setCamera();

}

void mouseEvent (int button, int state, int x, int y) {
	if (DEBUG_MODE) cout << "MouseEvent " << button << " " << state << endl;
	MB_STATE[button] = state;
	mx = x;
	my = y;
}

void mouseMove (int x, int y) {

	if (MB_STATE[GLUT_RIGHT_BUTTON] == GLUT_DOWN) {

		zoom *= (y-my)/100.0 + 1;
		setCamera();

	} else {

		rotX += x - mx;
		rotY += y - my;
		setCamera();

	}

	mx = x;
	my = y;
	glutPostRedisplay();

}

void mouseMovePassive (int x, int y) {
	if (currentState == FIRST_PERSON_STATE) mouseMove(x,y);
}

void keyboardUp (unsigned char key, int x, int y) {
	DOWN[key] = false;
}

void keyboardEvent (unsigned char key, int x, int y) {

    int oldState = currentState;

    if (DEBUG_MODE) cout << (int)key << endl;

    DOWN[key] = true;

    switch((int)key) {

        case ESC_KEY: system("clear");
        	cout << "KTHXBYE" << endl << endl;
        	exit(0);
        	break;

        case D_KEY:
			if (currentState == OPTIONS_STATE) DEBUG_MODE = !DEBUG_MODE;
			break;

		case W_KEY:
			if (currentState == OPTIONS_STATE) {
				SPHERE_VISIBLE = !SPHERE_VISIBLE;
				glutPostRedisplay();
			}
			break;

		case A_KEY:
			if (currentState == OPTIONS_STATE) {
				AXIS_VISIBLE = !AXIS_VISIBLE;
				glutPostRedisplay();
			}
			break;

		case B_KEY:
			if (currentState == OPTIONS_STATE) {
				BOX_VISIBLE = !BOX_VISIBLE;
				glutPostRedisplay();
			}
			break;

		case C_KEY:
			if (currentState == INSPECT_STATE) {
				ORTHO_CAMERA = !ORTHO_CAMERA;
				setCamera();
				glutPostRedisplay();
			} else if (currentState == FIRST_PERSON_STATE) {
				THIRD_CAMERA = !THIRD_CAMERA;
				setCamera();
				glutPostRedisplay();
			}
			break;

		case R_KEY:
			if (currentState == INSPECT_STATE) {
				rotX = 0;
				rotY = 0;
				zoom = 1;
				ORTHO_CAMERA = true;
				setCamera();
				glutPostRedisplay();
			}
			break;

		case F_KEY:
			if (currentState == OPTIONS_STATE) {
				glutFullScreen();
			}
			break;

		case I_KEY:
			currentState = INSPECT_STATE;
			setCamera();
			glutPostRedisplay();
			break;

		case O_KEY:
			currentState = OPTIONS_STATE;
			break;

		case P_KEY:
			currentState = FIRST_PERSON_STATE;
			setCamera();
			glutPostRedisplay();
			break;

		case V_KEY:
			if (currentState == OPTIONS_STATE) {
				WALLS_VISIBLE = !WALLS_VISIBLE;
				glutPostRedisplay();
			}
			break;

		case SPACE_KEY:
			if (currentState == INSPECT_STATE) {
				randomVRP();
				setCamera();
				glutPostRedisplay();
			}
			break;

    }

   if (oldState != currentState) {
   		renderUI();
   }

}

void mainLoop (int v) {
	++d;
	if (currentState == FIRST_PERSON_STATE) {
		if (DOWN[W_KEY]) {
			gameobjects[0].p[0] += 0.1*sin(angle);
			gameobjects[0].p[2] += 0.1*cos(angle);
		}
		if (DOWN[A_KEY]) {
			gameobjects[0].p[0] += 0.1*sin(angle+M_PI/2.0);
			gameobjects[0].p[2] += 0.1*cos(angle+M_PI/2.0);
		}
		if (DOWN[D_KEY]) {
			gameobjects[0].p[0] += 0.1*sin(angle-M_PI/2.0);
			gameobjects[0].p[2] += 0.1*cos(angle-M_PI/2.0);
		}
		if (DOWN[S_KEY]) {
			gameobjects[0].p[0] += 0.1*sin(angle+M_PI);
			gameobjects[0].p[2] += 0.1*cos(angle+M_PI);
		}
		setCamera();
	}
	glutPostRedisplay();
	glutTimerFunc(1000/60, mainLoop, 0);
}

int main (int argc, const char * argv []) {

	srand (time(NULL));

	states.push_back("Inspect! (I)");
	states.push_back("Options! (O)");
	states.push_back("First Person! (P)");

	//PATRICKS
	GameModel patrickModel (PATRICK_MODEL_PATH);



	GameObject patrick = GameObject(patrickModel);

	patrick.p[0] = 0;
	patrick.p[1] = 0;
	patrick.p[2] = 0;

	patrick.s[0] = patrick.s[1] = patrick.s[2] = 1;

	patrick.regPoint[0] = 0;
	patrick.regPoint[1] = -1;
	patrick.regPoint[2] = 0;

	gameobjects.push_back(patrick);



	patrick = GameObject(patrickModel);

	patrick.p[0] = 2.5;
	patrick.p[1] = 0;
	patrick.p[2] = -2.5;
	patrick.s[0] = patrick.s[1] = patrick.s[2] = 1.5;

	patrick.r[1] = 90;

	patrick.regPoint[0] = 0;
	patrick.regPoint[1] = -1;
	patrick.regPoint[2] = 0;

	gameobjects.push_back(patrick);



	//FLOOR

	float height = 0;

	vector<Vertex> v = vector<Vertex>(3);
	v[0] = -floor_size/2.0;
	v[1] = -height;
	v[2] = -floor_size/2.0;
	floorVertex.push_back(v);

	v = vector<Vertex>(3);
	v[0] = -floor_size/2.0;
	v[1] = -height;
	v[2] = floor_size/2.0;
	floorVertex.push_back(v);

	v = vector<Vertex>(3);
	v[0] = floor_size/2.0;
	v[1] = -height;
	v[2] = floor_size/2.0;
	floorVertex.push_back(v);

	v = vector<Vertex>(3);
	v[0] = floor_size/2.0;
	v[1] = -height;
	v[2] = -floor_size/2.0;
	floorVertex.push_back(v);



	//Wall critical vertices
	v = vector<Vertex>(3);
	v[0] = -5;
	v[1] = 1.5;
	v[2] = -5;
	wallVertex.push_back(v);

	v = vector<Vertex>(3);
	v[0] = -5;
	v[1] = 1.5;
	v[2] = 5;
	wallVertex.push_back(v);

	v = vector<Vertex>(3);
	v[0] = 4.5;
	v[1] = 1.5;
	v[2] = 1.4;
	wallVertex.push_back(v);

	v = vector<Vertex>(3);
	v[0] = 4.5;
	v[1] = 1.5;
	v[2] = 1.6;
	wallVertex.push_back(v);

	randomVRP();

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
	glutPassiveMotionFunc(mouseMovePassive);
	glutKeyboardFunc(keyboardEvent);
	glutKeyboardUpFunc(keyboardUp);

	renderUI();

	glutTimerFunc(1000/60, mainLoop, 0);
	glutMainLoop();

  return 0;
}
