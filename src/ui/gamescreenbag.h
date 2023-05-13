#ifndef GAMESCREENBAG_H
#define GAMESCREENBAG_H

#include <gamescreen.h>


class GameScreenBag : public GameScreen
{
	public:
		GameScreenBag();

		virtual GameScreens events(sf::Event &event, sf::RenderWindow &window);
		virtual GameScreens otherEvents();
		virtual void update(sf::Time deltaTime);
		virtual void render(sf::RenderWindow &app);

	protected:
		GameScreens currentScreen;
	private:
};

#endif // GAMESCREENBAG_H
