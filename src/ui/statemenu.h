#ifndef STATEMENU_H
#define STATEMENU_H

#include <state.h>


class StateMenu : public State
{
public:
	virtual States run(sf::RenderWindow &app);
protected:

};

#endif // STATEMENU_H
