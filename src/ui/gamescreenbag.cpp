#include "gamescreenbag.h"

GameScreenBag::GameScreenBag() :
	currentScreen(GameScreens::BAG){}

GameScreens GameScreenBag::events(sf::Event &event, sf::RenderWindow &window) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
			case sf::Keyboard::I :
				Log::info("Close inventory");
				return GameScreens::PLATFORMS;
				break;
			default:
				break;
		}
	}
	return currentScreen;
}

GameScreens GameScreenBag::otherEvents() {
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        std::cout << "up" << std::endl;
    }*/
    return currentScreen;
}

void GameScreenBag::update(sf::Time deltaTime) {
}

void GameScreenBag::render(sf::RenderWindow &app) {
	app.clear(sf::Color(75, 35, 15, 255));
	//window.draw(text);//рисую этот текст
}
