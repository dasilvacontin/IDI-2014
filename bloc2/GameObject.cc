#include "GameObject.h"
#include <iostream>
using namespace std;

GameObject::GameObject (GameModel &gm) {

	this->initializeVectors();
	this->gm = gm;

}

void GameObject::initializeVectors () {

	for (int i = 0; i < 3; ++i) {
		this->p[i] = 0;
	}

}
