#include "db.h"

int Db::getPlayerId() {
	sqlArray settings = Sql::connect().getData("SELECT * FROM Settings;");
	return std::stoi(settings[0]["playerId"]);
}

int Db::getActiveGameScreen() {
	sqlParams playerIdVect {std::to_string(getPlayerId())};
	//Log::info(std::to_string(getPlayerId()));
	sqlArray profile = Sql::connect().getData("SELECT * FROM Profile WHERE id=?", playerIdVect );
	return std::stoi( profile[0]["gameScreen"] );
}

void Db::setActiveGameScreen(int screen){
	sqlParams v {std::to_string(screen), std::to_string(getPlayerId())};
	Sql::connect().request("UPDATE Profile SET gameScreen=? WHERE id=?", v);
}

bool Db::hasMap() {
    //Log::info("check map for player");
	sqlParams playerIdVect {std::to_string(getPlayerId())};
	sqlArray setting = Sql::connect().getData("SELECT * FROM Profile WHERE id=?", playerIdVect );
	int moveSetting = std::stoi( setting[0]["hasMap"] );
	return moveSetting == 0 ? false : true;
}

void Db::toggleProfileMap(bool state){
	std::string newState = state ? "1" : "0";
	sqlParams v {newState, std::to_string(getPlayerId())};
	Sql::connect().request("UPDATE Profile SET hasMap=? WHERE id=?", v);
}

void Db::addGroundTile(std::vector<std::vector<int>> mapArray) {
	std::string request = "INSERT INTO MapGround (playerId,x,y,type) VALUES ";
	int i = 0;
	for (std::vector<int> &m : mapArray) {
		if (i != 0) {
			request += ",";
		}
		request += "("
			+ std::to_string(getPlayerId()) + ","
			+ std::to_string(m[0]) + ","
			+ std::to_string(m[1]) + ","
			+ std::to_string(m[2]) + ")";
		i++;
	}
	request += ";";
	Sql::connect().request(request);
}

std::vector<std::vector<int>> Db::getGroundTile() {
	sqlParams playerIdVect {std::to_string(getPlayerId())};
	sqlArray mapArray = Sql::connect().getData("SELECT * FROM MapGround WHERE playerId=?", playerIdVect );
	//int moveSetting = std::stoi( setting[0]["hasMap"] );
	std::vector<std::vector<int>> resultArray;
	for (std::map <std::string, std::string> &m : mapArray) {
		std::vector<int> currentTile;
		currentTile.push_back(std::stoi( m["x"] ));
		currentTile.push_back(std::stoi( m["y"] ));
		currentTile.push_back(std::stoi( m["type"] ));
		resultArray.push_back(currentTile);
	}
	return resultArray;
}

void Db::makeHero(int x, int y) {
	sqlParams v {std::to_string(getPlayerId()), std::to_string(x), std::to_string(y)};
	Sql::connect().request("INSERT INTO Hero (playerId, x, y) VALUES (?, ?, ?)", v);
}

void Db::setHeroCoord(int x, int y, bool isLookingRight) {
	int lookRight = 0;
	if (isLookingRight) {
		lookRight = 1;
	}
	sqlParams v {std::to_string(x), std::to_string(y), std::to_string(lookRight), std::to_string(getPlayerId())};
	Sql::connect().request("UPDATE Hero SET x=?, y=?, lookRight=? WHERE playerId=?", v);
}

int Db::getHeroCoordX() {
	sqlParams playerIdVect {std::to_string(getPlayerId())};
	sqlArray coord = Sql::connect().getData("SELECT * FROM Hero WHERE id=?", playerIdVect );
	return std::stoi( coord[0]["x"] );
}

int Db::getHeroCoordY() {
	sqlParams playerIdVect {std::to_string(getPlayerId())};
	sqlArray coord = Sql::connect().getData("SELECT * FROM Hero WHERE id=?", playerIdVect );
	return std::stoi( coord[0]["y"] );
}

bool Db::getHeroLookingRight() {
	sqlParams playerIdVect {std::to_string(getPlayerId())};
	sqlArray lookRight = Sql::connect().getData("SELECT * FROM Hero WHERE id=?", playerIdVect );
	if (lookRight[0]["lookRight"] == "1" ) {
		return true;
	} else {
		return false;
	}
}
