#ifndef HERO_H
#define HERO_H

#include <string>
#include <SFML/Audio.hpp>
#include "mapmanager.h"
#include "animator.h"

class Hero
{
	public:
		Hero();

		enum class State {
			STAY,
			GO,
			STAIR_UP,
			STAIR_DOWN
		};

		enum class Direction {
			RIGHT,
			LEFT,
			UP,
			DOWN,
		};

		Animator picture;

		std::string getImg() const;
		void setCoord(int x, int y);
		int getCoordX() const;
		int getCoordY() const;
		int getWidth() const;
		int getHeight() const;
		void setDirection(Direction dir, bool isActive, const MapManager & map);
		void update(int time, const MapManager & map);
		void switchRun(bool isActive);
		void save();

	protected:

	private:

		std::string heroTexture;
		int x,y;
		float speed;
		bool movingLeft, movingRight, movingUp, movingDown;
		State state;
		//Direction dir;
		bool isLookingRight;
		bool isRunning;

		struct NextCell {
			int x;
			int y;
		};
		std::vector<NextCell> path;

		int nextStair;

		void move(int time, const MapManager & map);
		void setStairState(const MapManager & map);

		sf::Sound movingSound;
};

#endif // HERO_H
