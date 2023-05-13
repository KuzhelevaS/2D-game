#ifndef STATEGAME_H
#define STATEGAME_H

#include <state.h>

class StateGame : public State
{
public:
	virtual States run(sf::RenderWindow &app);
protected:
};

#endif // STATEGAME_H
