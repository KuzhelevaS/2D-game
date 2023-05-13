#ifndef DB_H
#define DB_H

#include "sql.h"

class Db
{
	public:
		static int getPlayerId();

		static int getActiveGameScreen();
		static void setActiveGameScreen(int screen);

		static bool hasMap();
		static void toggleProfileMap(bool state);
		static void addGroundTile(std::vector<std::vector<int>> mapArray);
		static std::vector<std::vector<int>> getGroundTile();

		static void makeHero(int x, int y);
		static void setHeroCoord(int x, int y, bool isLookingRight);
		static int getHeroCoordX();
		static int getHeroCoordY();
		static bool getHeroLookingRight();

	protected:

	private:
};

#endif // DB_H
