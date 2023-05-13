#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <SFML/Graphics.hpp>

class Animator {
	public:
		Animator(int originX = 0, int originY = 0);
		//void print(sf::RenderWindow &window, int width, int height, int x, int y);

		void addAnimation(std::string name, std::string textureFile,
			int x, int y, int width, int height, int frameCount, int frameInRow, int duration,
			bool loop = true, bool mirror = false, int delay = 0,
			int ySwingSize = 0, int xSwingSize = 0);
		void switchAnimation(std::string name);
		void update(int dt, int x, int y); //dt in milliseconds
		void setCoords(int x, int y);
		sf::Sprite& getSprite();

	private:
		struct Animation {
			std::string name, textureFile;
			int textureX, textureY, textureW, textureH, maxFrame, framePerRow, duration, delay; //duration, delay in milliseconds
			bool isLoop, isMirror, isRunning;
			int hasYswing, hasXswing;
		};
		std::map<std::string,Animation> animationList;
		std::string currentAnimation;

		int originX, originY;
		sf::Sprite animationSprite;
		int animationTime;
};


#endif // Animator_H
