#include "statemanager.h"

void StateManager::run(sf::RenderWindow &app) {
	States s = States::MENU;

	StateMenu stateMenu;
	StateGame stateGame;

	Log::info("Run StateManager");

	//Main loop
	State * currentState;
	while (s != States::END)
	{
		switch (s) {
			case States::GAME :
				currentState = &stateGame;
				break;
			case States::MENU :
				currentState = &stateMenu;
				break;
			default :
				break;
		}
		s = currentState->run(app);
	}

}
