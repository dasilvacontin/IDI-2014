#ifndef GAME_MODEL
#define GAME_MODEL

#include "../Model/model.h"
#include <string>
#include <vector>

class GameModel {

	Model _m;
	std::vector <Vertex> _p;
	std::vector <Vertex> _r;
	std::vector <Vertex> _s;

	public:
		GameModel();
		GameModel(const std::string &path_name);

		const std::vector<Vertex>& p() const;

	private:
		void initializeVectors();
		void normalizeModel();

};

#endif
