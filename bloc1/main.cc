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
using namespace std;

int mx = 0, my = 0, edge = 600, rot = 0;
float xView = 0, yView = 0, brightness = 1;

const int H_KEY = 104;
const int ESC_KEY = 27;
const char* WINDOW_NAME = "Hello World";
int windowIndentifier;

const int BG_STATE = 0;
const int TRIANGLE_STATE = 1;
const int BG_STATE_KEY = 98; //'b'
const int TRIANGLE_STATE_KEY = 116; //'t'
int currentState = BG_STATE;

struct P {
  float x, y, z;
  P (float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }
  void log () {
    cout << "Point (" << this->x << ", " << this->y << ", " << this->z << ")" << endl << endl;
  }
};

vector<P> points;

void renderScene (void) {
    glClearColor(0.7*brightness,0.2*brightness,0.5*brightness,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    //glTranslatef((mx-xView)/(edge/2.0) - 1, 1 - (my-yView)/(edge/2.0), 0);
    //glRotatef(++rot, 0, 0, 1);

    glBegin(GL_TRIANGLES);
      for (int i = 0; i < points.size(); ++i) {
        P &p = points[i];
        glColor4f(0.3*(i%3+1),0.3*(i%3+1),1,1);
        glVertex3f(p.x, p.y, p.z);
      } 
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

P pointForClick (int x, int y) {
  x -= xView;
  y -= yView;
  return P (((float)2*x/edge)-1, -1*(((float)2*y/edge)-1), 0);
}

void mouseEvent (int x, int y, bool click) {
    //mx = x;
    //my = y;
    switch (currentState) {
      case BG_STATE: brightness = (float) x/edge;
        break;
      case TRIANGLE_STATE: if (click) {
          P p = pointForClick(x,y);
          p.log();
          points.push_back(p);
        }
        break;
    }
    
    glutPostRedisplay();
}

void mouseClick (int button, int state, int x, int y) {
    if (state == GLUT_DOWN) mouseEvent(x, y, true);
}

void mouseMove (int x, int y) {
    mouseEvent(x, y, false);
}

void showHelp () {
  cout << "Welcome!" << endl << endl;
  cout << "> 'B' -> Background Color Mode" << endl;
  cout << "> 'T' -> Triangle Creation Mode" << endl;
  cout << "> 'H' -> Help" << endl;
  cout << endl;
  cout << "ESC key to quit." << endl;
  cout << endl;
}

void closeApp () {
  cout << "Sayonara!" << endl << endl;
  glutDestroyWindow(windowIndentifier);
  exit(0);
}

void keyboardEvent (unsigned char key, int x, int y) {

  //cout << "Key: " << (int)key << ", " << key << endl;

  switch ((int)key) {

    case H_KEY: showHelp();
      break;
    case ESC_KEY: closeApp();
      break;

    case BG_STATE_KEY: currentState = BG_STATE;
      cout << "Background Color Mode!" << endl << endl;
      break;
    case TRIANGLE_STATE_KEY: currentState = TRIANGLE_STATE;
      cout << "Triangle Creation Mode!" << endl << endl;
    break;

  }

}

int main (int argc, const char * argv []) {

  system("clear");
  cout << endl;
  showHelp();

  points.push_back(P(-1/2.0, -1/3.0, 0));
  points.push_back(P(1/2.0, -1/3.0, 0));
  points.push_back(P(0, 2/3.0, 0));

  glutInit(&argc, (char **)argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(edge,edge);
  windowIndentifier = glutCreateWindow(WINDOW_NAME);
  glutReshapeFunc(reshapeWindow);
  glutDisplayFunc(renderScene);
  glutMouseFunc(mouseClick);
  glutMotionFunc(mouseMove);
  glutKeyboardFunc(keyboardEvent);
  glutMainLoop();
  return 0;
}
