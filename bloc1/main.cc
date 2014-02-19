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
using namespace std;

int mx = 0, my = 0, edge = 600, rot = 0;
float xView = 0, yView = 0;

void renderScene (void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    glTranslatef((mx-xView)/300.0 - 1, 1 - (my-yView)/300.0, 0);
    glRotatef(++rot, 0, 0, 1);
    glBegin(GL_TRIANGLES);
        glVertex3f(-0.5,-1/3.0,0);
        glVertex3f(0.5,-1/3.0,0);
        glVertex3f(0.0,2/3.0,0);
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

void mouseClick (int button, int state, int x, int y) {
}

void mouseMove (int x, int y) {
    mx = x;
    my = y;
    glutPostRedisplay();
}

int main (int argc, const char * argv []) {
  glutInit(&argc, (char **)argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(edge,edge);
  glutCreateWindow("hello world");
  glutReshapeFunc(reshapeWindow);
  glutDisplayFunc(renderScene);
  glutMouseFunc(mouseClick);
  glutMotionFunc(mouseMove);
  glutMainLoop();
  return 0;
}
