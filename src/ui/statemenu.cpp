#include "statemenu.h"
#include "sql.h"

States StateMenu::run(sf::RenderWindow &app) {
	Log::info("State Menu");

	//TODO Исключение, если файл невозможно записть
	//Предложить удалить существующий и создать с нуля.
	//Предупредить, что все данные могут быть потеряны

	while(app.isOpen())
    {
        sf::Event event;
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
				app.close();
                return States::END;
            }

            if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
					case sf::Keyboard::Escape:
						return (States::GAME);
						break;
					case sf::Keyboard::Q:
						app.close();
						return States::END;
						break;
					default:
						break;
				}
			}

        }

        app.clear(sf::Color(49, 127, 127, 255));
		//window.draw(text);//рисую этот текст
        app.display();
    }
}

