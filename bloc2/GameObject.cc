#include "GameObject.h"
#include <iostream>
using namespace std;

GameObject::GameObject (GameModel &gm) {

	initializeVectors();
	_gm = gm;

}

void GameObject::render () {
	
	glPushMatrix();

	glTranslated(p[0], p[1], p[2]);

	glRotated(r[0], 1, 0, 0);
	glRotated(r[1], 0, 1, 0);
	glRotated(r[2], 0, 0, 1);

	glScaled(s[0], s[1], s[2]);

	renderAxis(2);
	_gm.render();

	glPopMatrix();

}

void GameObject::initializeVectors () {

	p = std::vector<Vertex> (3, 0);
	r = std::vector<Vertex> (3, 0);
	s = std::vector<Vertex> (3, 1);

}

void GameObject::renderAxis (int size) {

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
