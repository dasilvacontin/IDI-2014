#include "GameModel.h"

GameModel::GameModel () {

	initializeVectors();

}

GameModel::GameModel (const std::string &model_path) {

	initializeVectors();
	_m.load(model_path);
	normalizeModel();

}

const std::vector<Vertex>& GameModel::p() const {
	return _p;
}

void GameModel::initializeVectors () {

	_p = std::vector<Vertex> (3, 0);
	_r = std::vector<Vertex> (3, 0);
	_s = std::vector<Vertex> (3, 1);
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

}
