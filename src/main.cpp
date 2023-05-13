#include <SFML/Graphics.hpp>
#include "statemanager.h"
#include <iostream>
#include "log.h"
#include "global.h"

int main()
{
	Log::info("Start app");
	#ifdef DEBUG
        sf::RenderWindow window(sf::VideoMode(840, 600), L"Выживание");
    #endif
    #ifndef DEBUG
        //std::vector<sf::VideoMode> availiableVideoModes;
        //availiableVideoModes = sf::VideoMode::getFullscreenModes();//availiableVideoModes.at(0)
        //sf::RenderWindow window(sf::VideoMode(availiableVideoModes.at(0)), L"Выживание", sf::Style::Fullscreen);
        sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode()), L"Выживание", sf::Style::Fullscreen);
        //sf::RenderWindow window(sf::VideoMode(availiableVideoModes.at(0)), L"Выживание", sf::Style::None);
        //sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode()), L"Выживание", sf::Style::None);
        //sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode()), L"Выживание");
        window.setPosition(sf::Vector2i(0, 0));
    #endif

    sf::Image image;
    if (image.loadFromFile("assets/icon.png")) {
        window.setIcon(128, 128, image.getPixelsPtr());
    }
    window.setFramerateLimit(240);
    //window.setVerticalSyncEnabled(true);

    StateManager sm;
    sm.run(window);

	Log::info("End app");

    return 0;
}
