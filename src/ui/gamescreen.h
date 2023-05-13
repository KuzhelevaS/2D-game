#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <SFML/Graphics.hpp>
#include "log.h"
#include "settings.h"

class GameScreen
{
	public:
		GameScreen();
		virtual ~GameScreen();

		virtual GameScreens events(sf::Event &event, sf::RenderWindow &window) = 0;
		virtual GameScreens otherEvents() = 0;
		virtual void update(sf::Time deltaTime) = 0;
		virtual void render(sf::RenderWindow &app) = 0;

	protected:

	private:
};

#endif // GAMESCREEN_H
