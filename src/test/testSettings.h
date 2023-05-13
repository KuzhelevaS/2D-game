#include "catch.hpp"
#include "settings.h"

TEST_CASE("Settings") {
    remove("testGS.db");
    Sql::connect("testGS.db");
    Settings::gs().setGameScreen(GameScreens::MAX);
    GameScreens gameScreen = Settings::gs().getGameScreen();
    REQUIRE(gameScreen == GameScreens::MAX);
}

