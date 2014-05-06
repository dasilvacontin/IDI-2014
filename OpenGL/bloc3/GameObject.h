#ifndef GAME_OBJECT
#define GAME_OBJECT

#include "GLIncludes.h"
#include "GameModel.h"

class GameObject {

	public:
		GameObject();
		GameObject(GameModel &gm);
		void render (bool shouldRenderAxis = false, bool shouldRenderBox = false);

		std::vector <Vertex> p;
		std::vector <Vertex> r;
		std::vector <Vertex> s;
		std::vector <Vertex> regPoint;
		GameModel _gm;

	private:
		void initializeVectors();
		void renderAxis(int size);

};

#endif
