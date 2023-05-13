#ifndef GLOBALMAP_H
#define GLOBALMAP_H

#include <vector>

namespace GlobalMap {
	enum class Tiles {
		EMPTY,
		SOLID,
		STAIR,
		//STAIR_TOP,
		GRASS,
		STONE,
		WATER
	};

	using tileGround = std::vector<std::vector<Tiles>>;
};

#endif // GLOBALMAP_H
