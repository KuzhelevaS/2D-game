#include "catch.hpp"
#include "mapgenerator.h"

TEST_CASE("MapGenerator") {
	//MapGenerator m(360,60);
	//MapGenerator m(120,20);
	MapGenerator m(20,10);
	m.printMapGround();
	m.printHeroCoord();
}

