#ifndef GAMESCREENPLATFORMS_H
#define GAMESCREENPLATFORMS_H

#include "gamescreen.h"
#include "mapmanager.h"
#include "settings.h"
#include "hero.h"
#include "animator.h"

class GameScreenPlatforms : public GameScreen
{
	public:
		GameScreenPlatforms(sf::RenderWindow &window);
		~GameScreenPlatforms();

		virtual GameScreens events(sf::Event &event, sf::RenderWindow &window);
		virtual GameScreens otherEvents();
		virtual void update(sf::Time deltaTime);
		virtual void render(sf::RenderWindow &window);

	protected:
		GameScreens currentScreen;
	private:
		MapManager map;
		sf::Texture groundImg;
		sf::Sprite groundSprite;
		void drawGround(sf::RenderWindow &window);

		Hero hero;
		sf::Texture heroImg;
		sf::Sprite heroSprite;
		void drawHero(sf::RenderWindow &window);

		sf::View view;
		void setViewOnHero();

		Animator flow;

};

#endif // GAMESCREENPLATFORMS_H
