#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "GameModel.h"

class GameObject {

	GameModel gm;
	float p[3];
	float r[3];
	float s[3];

	public:
		GameObject(GameModel &gm);

	private:
		void initializeVectors();

};

#endif
