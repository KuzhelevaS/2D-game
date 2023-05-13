#include "stategame.h"
#include "gamescreenplatforms.h"
#include "gamescreenbag.h"

States StateGame::run(sf::RenderWindow &app) {
	Log::info("State Game");
	GameScreens s = Settings::gs().getGameScreen();
	GameScreen * currentScreen;

    Log::info("Start init Game Screens");

	GameScreenPlatforms platform(app);
	GameScreenBag bag;

	sf::Clock clock;
	//sf::Time prevTime = clock.restart();
	while(app.isOpen()) {
		sf::Event event;

		GameScreens temp;
		switch (s) {
			case GameScreens::PLATFORMS :
				currentScreen = &platform;
				break;
			case GameScreens::BAG :
				currentScreen = &bag;
				break;
			default :
				break;
		}

		while (app.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				app.close();
				return States::END;
			}

			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
					case sf::Keyboard::Escape:
						return (States::MENU);
						break;
					default:
						break;
				}
			}
			temp = currentScreen->events(event, app);
			if (temp != s) {
				break;
			}
		}
		if (temp == s) {
			temp = currentScreen->otherEvents();
		}
		if (temp != s) {
			s = temp;
			Settings::gs().setGameScreen(s);
			continue;
		}

		//sf::Time deltaTime = clock.restart();
//		if (clock.getElapsedTime().asMicroseconds() > prevTime.asMicroseconds()) {
//			prevTime = clock.restart();
//			currentScreen->update(prevTime);
//		}
		sf::Time deltaTime = clock.restart();
		currentScreen->update(deltaTime);
		currentScreen->render(app);
		app.display();

    }
}



