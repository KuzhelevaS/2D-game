#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include <string>
#include "mapgenerator.h"
#include "db.h"

class MapManager
{
	public:
		struct tile {
			int x;
			int y;
			int w;
			int h;
		};

		struct groundCell {
			int x;
			int y;
			GlobalMap::Tiles type;
			tile texture;
			bool isVisited = false;
		};

		MapManager();

		std::string getGroundImg();

		GlobalMap::Tiles getGroundCellType (int x, int y) const;

		std::vector<groundCell> groundList;

	protected:

	private:
		std::string groundTexture;
};

#endif // MAPMANAGER_H
