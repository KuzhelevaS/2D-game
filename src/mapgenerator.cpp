#include "mapgenerator.h"
#include "log.h"

using namespace GlobalMap;

MapGenerator::MapGenerator(int width, int height):
	areaSizeX(width), areaSizeY(height),
	tileMapGround(areaSizeX, std::vector<Tiles> (areaSizeY, Tiles::EMPTY)) {

    Log::info("MapGenerator constructor");
	generateMapGround();
	generateHeroCoord();
}

MapGenerator::~MapGenerator()
{
	//dtor
}

void MapGenerator::generateMapGround() {
    Log::info("MapGenerator make ground");
	int border = 1;
	int minX = border;
	int minY = border;
	int maxX = areaSizeX - border - 1;
	int maxY = areaSizeY - border - 1;

	std::vector<Room> rooms;

	rooms.push_back({minX, minY, maxX, maxY});

	//Разбиваем на комнаты
	Log::info("MapGenerator make ground - step rooms");
	while (rooms.size() > 0) {
		int minW = Global::platformMinW;
		int minH = Global::platformMinH;
		Room r = rooms.back();
		if (r.w > minW * 2 || r.h > minH * 2) {
			Room tempLeft;
			Room tempRight;
			if (r.w >= r.h && r.w > minW * 2) {
				int splitW = Global::intRand(minW, r.w - minW);
				tempLeft = {r.x, r.y, splitW, r.h};
				tempRight = {r.x + splitW, r.y, r.w - splitW, r.h};
			} else if (r.h > minH * 2) {
				int splitH = Global::intRand(minH, r.h - minH);
				tempLeft = {r.x, r.y, r.w, splitH};
				tempRight = {r.x, r.y + splitH, r.w, r.h - splitH};
			} else {
				readyRooms.push_back(r);
				rooms.pop_back();
				continue;
			}
			rooms.pop_back();
			rooms.push_back(tempLeft);
			rooms.push_back(tempRight);
		} else {
			readyRooms.push_back(r);
			rooms.pop_back();
		}

	}

	//Делаем горизонтальные плитки
	Log::info("MapGenerator make ground - step Hplates");
	int roomId = 0;
	for (Room &r : readyRooms) {
		int randY = Global::intRand(r.y, r.y + r.h - 1);
		r.resultH = randY;
		for(int x=r.x;x<r.x + r.w;x++) {
			tileMapGround[x][r.resultH] = Tiles::SOLID;
		}
		r.id = roomId;
		roomId++;
	}

	//Определяем граничащие комнаты
	Log::info("MapGenerator make ground - step neighbours");
	for (Room &ri : readyRooms) {
		int xi1 = ri.x;
		int xi2 = ri.x + ri.w;
		int yi1 = ri.y;
		int yi2 = ri.y + ri.h;
		for (Room &rj : readyRooms) {
			if (rj.id != ri.id && std::find(ri.neighbours.begin(), ri.neighbours.end(), rj.id) == ri.neighbours.end()) {
				int xj1 = rj.x;
				int xj2 = rj.x + rj.w;
				int yj1 = rj.y;
				int yj2 = rj.y + rj.h;

				bool isNeighbour = false;
				//Если крайние координаты X совпали
				if ((xi1 == xj2 || xj1 == xi2)
					//И хотя бы одна горизонтальная граница первого внутри горизонтальной границы второго
					&& ((yi1 >= yj1 && yi1 < yj2) || (yi2 >= yj1 && yi2 < yj2)
						|| (yj1 >= yi1 && yj1 < yi2) || (yj2 >= yi1 && yj2 < yi2) )) {
					isNeighbour = true;
				}

				//Если крайние координаты Y совпали
				if ((yi1 == yj2 || yj1 == yi2)
					//И хотя бы одна вертикальная граница первого внутри вертикальной границы второго
					&& ((xi1 >= xj1 && xi1 < xj2) || (xi2 >= xj1 && xi2 < xj2)
						|| (xj1 >= xi1 && xj1 < xi2) || (xj2 >= xi1 && xj2 < xi2) )) {
					isNeighbour = true;
				}

				if ((yi1 == yj2 || yj1 == yi2) && (xi1 == xj2 || xj1 == xi2)) {
					isNeighbour = false;
				}

				if (isNeighbour) {
					ri.neighbours.push_back(rj.id);
					rj.neighbours.push_back(ri.id);
				}
			}
		}
	}

	//Связываем все комнаты между собой
	Log::info("MapGenerator make ground - step binding");
	std::vector<Room> stackRooms;
	readyRooms[0].visited = true;
	Room currentRoom = readyRooms[0];
	int countVisited = 1;
	while (countVisited < readyRooms.size()) {
		std::vector<int> unvisited;
		//Получаем список непосещенных соседей
		if (currentRoom.neighbours.size() > 0) {
			for (int n : currentRoom.neighbours) {
				for (Room r : readyRooms) {
					if (r.id == n) {
						if (r.visited == false) {
							unvisited.push_back(n);
						}
						break;
					}
				}
			}

		}

		//Если есть непосещенные соседи
		if (unvisited.size() > 0) {
			stackRooms.push_back(currentRoom);
			int randNeighbour = Global::intRand(0, unvisited.size() - 1);
			for (Room &r : readyRooms) {
				if (r.id == unvisited[randNeighbour]) {
					//Строим лестницу если платформы не на одном уровне
					if (r.resultH != currentRoom.resultH) {
						Room bottom;
						Room top;
						if (r.resultH > currentRoom.resultH) {
							bottom = r;
							top = currentRoom;
						} else {
							bottom = currentRoom;
							top = r;
						}


						bool hasStair = false;
						//Проверяем, построена ли параллельная лестница
						/*int checkingX = bottom.x;
						while (tileMapGround[checkingX][bottom.resultH] != Tiles::EMPTY) {
							int distanse = bottom.resultH - top.resultH;
							int lenth = 0;
							for (int y = top.resultH; y < bottom.resultH; y++) {
								if (tileMapGround[checkingX][y] == Tiles::STAIR) {
									std::cout << "find stair x:" << checkingX << " y:" << y << std::endl;
									lenth++;
								}
							}
							if (lenth == distanse) {
								hasStair = true;
								break;
							}
							checkingX--;
						}

						checkingX = bottom.x + bottom.w - 1;
						while (tileMapGround[checkingX][bottom.resultH] != Tiles::EMPTY) {
							int distanse = bottom.resultH - top.resultH;
							int lenth = 0;
							for (int y = top.resultH; y < bottom.resultH; y++) {
								if (tileMapGround[checkingX][y] == Tiles::STAIR) {
									std::cout << "find stair x:" << checkingX << " y:" << y << std::endl;
									lenth++;
								}
							}
							if (lenth == distanse) {
								hasStair = true;
								break;
							}
							checkingX++;
						}*/

						//Если лестница не построена, генерируем
						if (!hasStair) {
							struct Closest {
								int x;
								int y;
							};
							std::vector<Closest> closestTop;
							if (bottom.x == top.x + top.w) {
								closestTop.push_back({top.x + top.w, top.resultH});
							} else if (top.x == bottom.x + bottom.w) {
								closestTop.push_back({top.x - 1, top.resultH});
							} else {
								for (int x = bottom.x; x < bottom.x + bottom.w; x++) {
									if (tileMapGround[x][top.resultH] == Tiles::SOLID) {
										closestTop.push_back({x, top.resultH});
									}
								}
							}

							Closest result;
							if (closestTop.size() == 1) {
								result = {closestTop[0].x, closestTop[0].y};
							} else if (closestTop.size() > 1) {
								int random = Global::intRand(0, closestTop.size() - 1);
								result = {closestTop[random].x, closestTop[random].y};
							}

							//Рисуем лестницу
							for (int y = bottom.resultH - 1; y >= result.y; y--) {
								tileMapGround[result.x][y] = Tiles::STAIR;
							}
						}
					}

					r.visited = true;
					currentRoom = r;
					countVisited++;
					break;
				}
			}
		} else {
			currentRoom = stackRooms.back();
			stackRooms.pop_back();
		}
	}

	//Расставляем биомы
	Log::info("MapGenerator make ground - step biomes");
	for(int y=0;y<areaSizeY;y++){
        for(int x=0;x<areaSizeX;x++) {
			int length = Global::intRand(1,5);
			std::vector<Tiles> biomes;
			biomes.push_back(Tiles::GRASS);
			biomes.push_back(Tiles::STONE);
			biomes.push_back(Tiles::WATER);
			if (tileMapGround[x][y] == Tiles::SOLID) {
				int type = Global::intRand(0,biomes.size() - 1);
				int biomeX = x;
				//Особые условия для воды
				if (biomes[type] == Tiles::WATER) {
					int realBiomeL = 0;

					int tempX = biomeX;
					int tempL = length;
					while (tempL > 0 && tileMapGround[tempX][y] == Tiles::SOLID) {
						if (tileMapGround[tempX][y - 1] == Tiles::STAIR) {
							break;
						}
						realBiomeL++;
						tempX++;
						tempL--;
					}

					bool isNeedChange = true;

					if ((tileMapGround[biomeX - 1][y] == Tiles::EMPTY)
						|| (tileMapGround[biomeX + realBiomeL][y] == Tiles::EMPTY)) {
						isNeedChange = false;
					}
					if (realBiomeL == 0) {
						isNeedChange = true;
					}

					if (isNeedChange) {
						while (biomes[type] == Tiles::WATER) {
							type = Global::intRand(0,biomes.size() - 1);
						}
					} else {
						length = realBiomeL;
					}
				}

				while (length > 0 && tileMapGround[biomeX][y] == Tiles::SOLID) {
					tileMapGround[biomeX][y] = biomes[type];
					biomeX++;
					length--;
				}
			}
		}
	}

	Log::info("MapGenerator make ground - end");

	//Отделяем верхние ступени лестницы
//	for(int y=0;y<areaSizeY;y++){
//		for(int x=0;x<areaSizeX;x++) {
//			if (tileMapGround[x][y] == Tiles::STAIR
//			&& (tileMapGround[x - 1][y] != Tiles::EMPTY
//			|| tileMapGround[x + 1][y] != Tiles::EMPTY)) {
//				tileMapGround[x][y] = Tiles::STAIR_TOP;
//			}
//		}
//	}
}

void MapGenerator::generateHeroCoord() {
    Log::info("MapGenerator make hero");
	int x = 0, y = 0;
	int edgeOffset = 5;
	do {
		x = Global::intRand(edgeOffset, tileMapGround.size() - 1 - edgeOffset);
		y = Global::intRand(edgeOffset, tileMapGround[x].size() - 1 - edgeOffset);
	} while(tileMapGround[x][y] == Tiles::EMPTY
		|| tileMapGround[x][y] == Tiles::WATER
		|| tileMapGround[x][y] == Tiles::STAIR
		|| tileMapGround[x][y - 1] == Tiles::STAIR);
	heroCoordX = x;
	heroCoordY = y;
}

int MapGenerator::getHeroCoordX() {
	return heroCoordX;
}

int MapGenerator::getHeroCoordY() {
	return heroCoordY;
}

void MapGenerator::printMapGround() {
	//Выводим соседей в консоль (для ручной проверки)
//	for (Room r : readyRooms) {
//		std::cout << "id: " << r.id << " x: " << r.x << "-" << r.x + r.w << " ";
//		std::cout << "y: " << r.y << "-" << r.y + r.h ;
//		std::cout << "; n: ";
//		for (int n : r.neighbours) {
//			std::cout << n << ",";
//		}
//		std::cout << std::endl;
//	}

//	std::cout<< "  ";
//	for(int x=0;x<areaSizeX;x++) {
//		std::cout<< x ;
//		if (x <= 9) {
//			std::cout << ' ' ;
//		}
//	}
//	std::cout<<std::endl;
	for(int y=0;y<areaSizeY;y++){
		//std::cout<< y << " ";
        for(int x=0;x<areaSizeX;x++) {
			switch (tileMapGround[x][y]) {
				case Tiles::EMPTY:
					std::cout << ' ';
					break;
				case Tiles::SOLID:
					std::cout << '=';
					break;
				case Tiles::STAIR:
					std::cout << '#';
					break;
//				case Tiles::STAIR_TOP:
//					std::cout << '+';
//					break;
				case Tiles::GRASS:
					std::cout << 'G';
					break;
				case Tiles::STONE:
					std::cout << 'S';
					break;
				case Tiles::WATER:
					std::cout << 'W';
					break;
			}
			//std::cout << ' ';
        }
        std::cout<<std::endl;
    }
}

void MapGenerator::printHeroCoord() {
	std::cout << "heroX " << heroCoordX << ", heroY " << heroCoordY << std::endl;
}

tileGround MapGenerator::getTileMapGround(){
	return tileMapGround;
}
