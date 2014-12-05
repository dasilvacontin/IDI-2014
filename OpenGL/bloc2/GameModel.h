#ifndef GAME_MODEL
#define GAME_MODEL

#include "GLIncludes.h"
#include "../Model/model.h"
#include <string>
#include <vector>

class GameModel {

	Model _m;
	std::vector <Vertex> _p;
	std::vector <Vertex> _r;
	std::vector <Vertex> _s;
	std::vector <Vertex> _box;

	public:
		GameModel();
		GameModel(const std::string &path_name);
		void render();
		void renderBox();

		const std::vector<Vertex>& p() const;
		const std::vector<Vertex>& box() const;

	private:
		void initializeVectors();
		void normalizeModel();
		void recursiveBoxLine(std::vector<int> mod);

};

#endif
