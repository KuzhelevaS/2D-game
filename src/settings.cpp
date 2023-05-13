#include "settings.h"
#include "log.h"

Settings::Settings()
{
}

Settings::~Settings()
{}

Settings & Settings::gs() {
	static Settings staticGS;
    return staticGS;
}

GameScreens Settings::getGameScreen() const {
	return static_cast<GameScreens>(Db::getActiveGameScreen());
}

void Settings::setGameScreen(GameScreens newScreen) {
	Db::setActiveGameScreen(static_cast<int>(newScreen));
}

