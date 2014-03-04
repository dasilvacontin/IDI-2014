#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "GLIncludes.h"
#include "GameModel.h"

class GameObject {

	GameModel _gm;

	public:
		GameObject(GameModel &gm);
		void render();

		std::vector <Vertex> p;
		std::vector <Vertex> r;
		std::vector <Vertex> s;

	private:
		void initializeVectors();
		void renderAxis(int size);

};

#endif
