#include "GameModel.h"

GameModel::GameModel () {

	this->initializeVectors();

}

GameModel::GameModel (const string &model_path) {

	this->initializeVectors();
	m.load(model_path);

}

void GameModel::initializeVectors () {

	for (int i = 0; i < 3; ++i) {
		this->p[i] = 0;
	}

}
