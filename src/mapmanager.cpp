#include "mapmanager.h"
#include "resourcemanager.h"

using namespace GlobalMap;

MapManager::MapManager()
	:groundTexture(asset::srcList.at("i_ground"))
{
    Log::info("MapManager constructor");

	std::vector<std::vector<int>> dbArray;
	if (Db::hasMap()) {
        Log::info("MapManager - Map exists");
		dbArray = Db::getGroundTile();
		for (std::vector<int> &m : dbArray) {
			tile CurrentTile;
			CurrentTile.w = Global::tileSize;
			CurrentTile.h = Global::tileSize;
			switch (static_cast<GlobalMap::Tiles>(m[2])) {
				case Tiles::STAIR:
					CurrentTile.x = 0 * Global::tileSize;
					CurrentTile.y = 0 * Global::tileSize;
					break;
				case Tiles::GRASS:
					CurrentTile.x = 0 * Global::tileSize;
					CurrentTile.y = 0 * Global::tileSize;
					CurrentTile.h = CurrentTile.h / 2;
					break;
				case Tiles::STONE:
					CurrentTile.x = 1 * Global::tileSize;
					CurrentTile.y = 0 * Global::tileSize;
					CurrentTile.h = CurrentTile.h / 2;
					break;
				case Tiles::WATER:
					CurrentTile.x = 2 * Global::tileSize;
					CurrentTile.y = 0 * Global::tileSize;
					CurrentTile.h = CurrentTile.h / 2;
					break;
				default:
					continue;
					break;
			}
			groundList.push_back({m[0] * Global::tileSize, m[1] * Global::tileSize, static_cast<GlobalMap::Tiles>(m[2]), CurrentTile});
		}
	} else {
	    Log::info("MapManager - Map not exists");
		MapGenerator generator(Global::fullWidth, Global::fullHeight);
		tileGround tileMapGround = generator.getTileMapGround();

		for(int y=0;y<Global::areaHeight;y++){
			for(int x=0;x<Global::areaWidth;x++) {
				tile CurrentTile;
				CurrentTile.w = Global::tileSize;
				CurrentTile.h = Global::tileSize;
				switch (tileMapGround[x][y]) {
					case Tiles::STAIR:
						CurrentTile.x = 0 * Global::tileSize;
						CurrentTile.y = 0 * Global::tileSize;
						break;
					case Tiles::GRASS:
						CurrentTile.x = 0 * Global::tileSize;
						CurrentTile.y = 0 * Global::tileSize;
						CurrentTile.h = CurrentTile.h / 2;
						break;
					case Tiles::STONE:
						CurrentTile.x = 1 * Global::tileSize;
						CurrentTile.y = 0 * Global::tileSize;
						CurrentTile.h = CurrentTile.h / 2;
						break;
					case Tiles::WATER:
						CurrentTile.x = 2 * Global::tileSize;
						CurrentTile.y = 0 * Global::tileSize;
						CurrentTile.h = CurrentTile.h / 2;
						break;
					default:
						continue;
						break;
				}
				groundList.push_back({x * Global::tileSize, y* Global::tileSize, tileMapGround[x][y], CurrentTile});

				std::vector<int> dbTile;
				dbTile.push_back(x);
				dbTile.push_back(y);
				dbTile.push_back(static_cast<int>(tileMapGround[x][y]));
				dbArray.push_back(dbTile);
			}
		}

		Db::addGroundTile(dbArray);
		Db::makeHero(generator.getHeroCoordX() * Global::tileSize + Global::tileSize / 2, generator.getHeroCoordY() * Global::tileSize);
		Db::toggleProfileMap(true);
	}

}

std::string MapManager::getGroundImg() {
	return groundTexture;
}

GlobalMap::Tiles MapManager::getGroundCellType (int x, int y) const {
	int cornerX = x - (x % Global::tileSize);
	int cornerY = y - (y % Global::tileSize);
	for (const MapManager::groundCell &g : groundList) {
		if (g.x == cornerX && g.y == cornerY) {
			return g.type;
		}
	}
	return GlobalMap::Tiles::EMPTY;
}
