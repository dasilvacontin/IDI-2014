#include "GameModel.h"
#include <iostream>

#pragma mark Public Methods
 
GameModel::GameModel () {

	initializeVectors();

}

GameModel::GameModel (const std::string &model_path) {

	initializeVectors();
	_m.load(model_path);
	normalizeModel();

}

void GameModel::render () {

	glPushMatrix();

	glScaled(_s[0], _s[1], _s[2]);
	glTranslated(_p[0], _p[1], _p[2]);

	std::vector<Face> faces = _m.faces();

	glBegin(GL_TRIANGLES);

	for (int i = 0; i < faces.size(); ++i) {

		const Face &f = faces[i];
		const Material &mat = Materials[f.mat];

		for (int j = 0; j < f.v.size(); ++j) {

			const float *d = mat.diffuse;
			glColor4f(d[0],d[1],d[2],d[3]);
			glVertex3dv(&_m.vertices()[f.v[j]]);

		}

	}

	glEnd();

	glPopMatrix();

}

void GameModel::renderBox () {

	std::vector<int> mod (0);
	recursiveBoxLine(mod);

}

#pragma mark Getters

const std::vector<Vertex>& GameModel::p() const {
	return _p;
}

const std::vector<Vertex>& GameModel::box() const {
	return _box;
}

#pragma mark Private Methods

void GameModel::recursiveBoxLine(std::vector<int> mod) {

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

void GameModel::initializeVectors () {

	_p = std::vector<Vertex> (3, 0);
	_r = std::vector<Vertex> (3, 0);
	_s = std::vector<Vertex> (3, 1);
	_box = std::vector<Vertex> (3, 0);
}

void GameModel::normalizeModel () {

	std::vector<Vertex> min (3);
	std::vector<Vertex> max (3);

	const std::vector<Vertex> &v = _m.vertices();

	//Init with first vertex
	for (int i = 0; i < 3; ++i) {
		min[i] = max[i] = v[i];
	}

	for (int i = 3; i < v.size(); ++i) {

		Vertex val = v[i];
		int coord = i%3;

		if (min[coord] > val) min[coord] = val;
		else if (max[coord] < val) max[coord] = val;

	}
	
	for (int i = 0; i < 3; ++i) {
		_p[i] = -(max[i]+min[i])/2.0;
	}

	
	//Fix scale
	Vertex maxEdge = max[0] - min[0];
	_box[0] = maxEdge;

	for (int i = 1; i < 3; ++i) {
		double edge = max[i] - min[i];
		_box[i] = edge;
		if (edge > maxEdge) maxEdge = edge;
	}

	double scale = 1.0 / maxEdge;
	for (int i = 0; i < 3; ++i) {
		_s[i] = scale;
		_box[i] *= scale;
	}
	
	/*
	std::cout << "Model Max Edge: " << maxEdge << ", Model scale: " << _s[0] << ", " << _s[1] << ", " << _s[2] << std::endl;
	*/

}
