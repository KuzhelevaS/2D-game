#include "animator.h"
#include "log.h"
#include "resourcemanager.h"

Animator::Animator(int originX, int originY)
	: originX(originX), originY(originY)
{
	//Log::info(std::to_string(this->originX) + "-" + std::to_string(this->originY));
	animationSprite.setOrigin(this->originX, this->originY);
}

void Animator::addAnimation(std::string name, std::string textureFile,
	int x, int y, int width, int height, int frameCount, int frameInRow, int duration,
	bool loop, bool mirror, int delay, int ySwingSize, int xSwingSize)
{
	if (!animationList.count(name)) {
		//Log::info("Add animation");
		Animation anim = {name, textureFile, x, y, width, height, frameCount, frameInRow,
			duration, delay, loop, mirror, true, ySwingSize, xSwingSize};
		animationList.insert({name, anim});
		if (animationList.size() == 1) {
			switchAnimation(name);
		}
	}
}

void Animator::switchAnimation(std::string name) {
	if (animationList.count(name) && currentAnimation != name) {
		//Log::info("Switch animation");
		currentAnimation = name;
		animationTime = 0;
		animationList.at(currentAnimation).isRunning = true;
		animationSprite.setTexture(ResourceManager::instance().useTexture(animationList.at(currentAnimation).textureFile));
	}
}

void Animator::update(int dt, int x, int y) {
	if (animationList.at(currentAnimation).isRunning) {
		//Вычисляем текущий фрейм
		//TODO Учитываем задержку
		animationTime += dt;
		int offsetX = animationList.at(currentAnimation).textureX;
		int offsetY = animationList.at(currentAnimation).textureY;
		if (animationList.at(currentAnimation).duration != 0) {
			int maxFrame = animationList.at(currentAnimation).maxFrame;
			int timePerFrame = animationList.at(currentAnimation).duration / maxFrame; //in milliseconds
			//Если подошло время перелистывать кадр
			if (animationTime >= timePerFrame) {
				int currentFrame = (animationTime / timePerFrame) % maxFrame;
				int rowLengh = animationList.at(currentAnimation).framePerRow;
				offsetX += currentFrame % rowLengh * animationList.at(currentAnimation).textureW;
				offsetY += (currentFrame / rowLengh)  * animationList.at(currentAnimation).textureH;
				if (currentFrame >= maxFrame || !animationList.at(currentAnimation).isLoop) {
					animationList.at(currentAnimation).isRunning = false;
				}
			}
		}
		//Задаем фрейм спрайту
		sf::IntRect rect = {
			offsetX, offsetY,
			animationList.at(currentAnimation).textureW,
			animationList.at(currentAnimation).textureH};
		animationSprite.setTextureRect(rect);
		//Отзеркаливаем, если нужно
		if (animationList.at(currentAnimation).isMirror) {
			animationSprite.setScale(-1, 1);
		} else {
			animationSprite.setScale(1, 1);
		}
		//Вычисляем покачивание, если есть
		//Задаем координаты спрайта с учетом покачивания
		animationSprite.setPosition(x, y);
	}
}

void Animator::setCoords(int x, int y) {
	animationSprite.setPosition(x, y);
}

sf::Sprite& Animator::getSprite() {
	return animationSprite;
}
