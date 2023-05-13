#ifndef STATE_H
#define STATE_H

#include <SFML/Graphics.hpp>
#include "log.h"

enum class States {
	MENU,
	//Создание профиля
	//Выбор профиля
	//Подтверждение новой игры
	//Подтверждение удаления профиля
	//Настройки
	//Информация
	GAME,
	END,
	MAX
};

class State
{
	public:
		State();
		virtual ~State();

		virtual States run (sf::RenderWindow &app) = 0;
	protected:
};

#endif // STATE_H
