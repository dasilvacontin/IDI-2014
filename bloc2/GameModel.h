#ifndef GAME_MODEL
#define GAME_MODEL

#include "../Model/model.h"
#include <string>
using namespace std;

class GameModel {

	Model m;
	float p[3];
	float r[3];
	float s[3];

	public:
		GameModel();
		GameModel(const string &path_name);

	private:
		void GameModel::initializeVectors();

};

#endif
