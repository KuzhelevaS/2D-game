#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "global.h"
#include "globalmap.h"

struct Room {
	int x;
	int y;
	int w;
	int h;
	int resultH;
	int id;
	std::vector<int> neighbours;
	bool visited = false;
};

class MapGenerator
{
	public:
		MapGenerator(int width, int height);
		~MapGenerator();

		void printMapGround();
		void printHeroCoord();
		GlobalMap::tileGround getTileMapGround();
		int getHeroCoordX();
		int getHeroCoordY();

	protected:

	private:
		int areaSizeX;
		int areaSizeY;
		GlobalMap::tileGround tileMapGround;
		std::vector<std::vector<int>> tileMapObject;
		std::vector<std::vector<int>> areaMap;
		int heroCoordX;
		int heroCoordY;

		std::vector<Room> readyRooms;

		void generateMapGround();
		void generateHeroCoord();

};

#endif // MAPGENERATOR_H
