#include "hero.h"
#include "db.h"
#include "global.h"
#include "resourcemanager.h"

Hero::Hero()
	: heroTexture(asset::srcList.at("i_hero")), x(Db::getHeroCoordX()), y(Db::getHeroCoordY()), speed(0.3),
	movingLeft(false), movingRight(false), movingUp(false), movingDown(false), isLookingRight(Db::getHeroLookingRight()), isRunning(false),
	state(Hero::State::STAY),  path(), picture(getWidth() / 2, getHeight())
{
	//dir(Direction::RIGHT),
	//if (!isLookingRight) {
	//	dir = Direction::LEFT;
	//}
	picture.addAnimation("StayToRight", heroTexture, 0, 0, getWidth(), getHeight(), 1, 1, 0, false);
	picture.addAnimation("StayToLeft", heroTexture, 0, 0, getWidth(), getHeight(), 1, 1, 0, false, true);
	picture.addAnimation("GoToRight", heroTexture, 0, getHeight(), getWidth(), getHeight(), 8, 8, 800);
	picture.addAnimation("GoToLeft", heroTexture, 0, getHeight(), getWidth(), getHeight(), 8, 8, 800, true, true);
	picture.addAnimation("FlyToRight", heroTexture, getWidth(), 0, getWidth(), getHeight(), 1, 1, 0);
	picture.addAnimation("FlyToLeft", heroTexture, getWidth(), 0, getWidth(), getHeight(), 1, 1, 0, false, true);
}

std::string Hero::getImg() const {
	return heroTexture;
}

int Hero::getWidth() const {
	return Global::tileSize * 8 / 10;
}

int Hero::getHeight() const {
	return Global::tileSize;
}

int Hero::getCoordX() const {
	return x;
}

int Hero::getCoordY() const {
	return y;
}

void Hero::setDirection(Direction dir, bool isActive, const MapManager & map) {
	if (dir == Direction::LEFT) {
		movingLeft = isActive;
	}
	if (dir == Direction::RIGHT) {
		movingRight = isActive;
	}
	if (dir == Direction::UP) {
		movingUp = isActive;
	}
	if (dir == Direction::DOWN) {
		movingDown = isActive;
	}

	if (this->state != State::STAIR_UP && this->state != State::STAIR_DOWN) {
		if (movingDown || movingUp || movingLeft || movingRight) {
			//this->state = State::GO;
			setStairState(map);
		} else {
			this->state = State::STAY;
		}
	}

	if (movingLeft && movingRight) {
		//do nothing
	} else if (movingLeft) {
		isLookingRight = false;
	} else if (movingRight) {
		isLookingRight = true;
	}
}

void Hero::switchRun(bool isActive) {
	isRunning = isActive;
}

void Hero::update(int time, const MapManager & map) {
	if (this->isRunning) {
		time *= 2;
	}
	move(time, map);
	picture.update(time, getCoordX(), getCoordY() + 8);
	std::string stepSound =  asset::srcList.at("s_step");
	switch (state) {
		case State::STAIR_UP :
		case State::STAIR_DOWN :
			movingSound.stop();
			switch (isLookingRight) {
				case false :
					picture.switchAnimation("FlyToLeft");
					break;
				case true :
					picture.switchAnimation("FlyToRight");
					break;
			}
			break;
		case State::GO :
			if (movingLeft && movingRight) {
				movingSound.stop();
				switch (isLookingRight) {
					case false :
						picture.switchAnimation("StayToLeft");
						break;
					case true :
						picture.switchAnimation("StayToRight");
						break;
				}
			} else if (movingLeft || movingRight) {
//				if (movingSound.getStatus() != sf::SoundSource::Status::Playing) {
//					movingSound.setBuffer(ResourceManager::instance().useSound(stepSound));
//					movingSound.setLoop(true);
//					movingSound.play();
//				}
//				if (isRunning) {
//					movingSound.setPitch(1.5f);
//				} else {
//					movingSound.setPitch(1.2f);
//				}
				switch (isLookingRight) {
					case false :
						picture.switchAnimation("GoToLeft");
						break;
					case true :
						picture.switchAnimation("GoToRight");
						break;
				}
			}
			break;
		case State::STAY :
		default:
			movingSound.stop();
			switch (isLookingRight) {
				case false :
					picture.switchAnimation("StayToLeft");
					break;
				case true :
					picture.switchAnimation("StayToRight");
					break;
			}
			break;
	}
}

void Hero::move(int time, const MapManager & map) {
	if (!path.empty()) {
	}

	int distanse = this->speed * time;
	if (this->state == State::STAIR_UP || this->state == State::STAIR_DOWN) {

		bool isEndOfStair = false;
		int newY;
		if (this->state == State::STAIR_UP) {
			newY = this->y - distanse;
			if (newY < this->nextStair) {
				newY = this->nextStair;
				isEndOfStair = true;
			}
		}
		if (this->state == State::STAIR_DOWN) {
			newY = this->y + distanse;
			if (newY > this->nextStair) {
				newY = this->nextStair;
				isEndOfStair = true;
			}
		}
		this->y = newY;
		if (isEndOfStair) {
			if (movingDown || movingUp || movingLeft || movingRight) {
				//his->state = State::GO;
				setStairState(map);
			} else {
				this->state = State::STAY;
			}
		}
	} else if (this->state == State::GO) {

		/*if (movingDown || movingUp) {
			GlobalMap::Tiles nextTile;
			if (movingUp) {
				nextTile = map.getGroundCellType(getCoordX(), getCoordY() - 1);
			} else if (movingDown) {
				nextTile = map.getGroundCellType(getCoordX(), getCoordY());
			}

			if (nextTile == GlobalMap::Tiles::STAIR) {
				if (movingUp) {
					nextStair = getCoordY() - Global::tileSize;
					this->state = State::STAIR_UP;
				} else if (movingDown) {
					nextStair = getCoordY() + Global::tileSize;
					this->state = State::STAIR_DOWN;
				}
			}
		}*/ //else {
			if (movingLeft) {
				int nextX = getCoordX() - getWidth() / 3.5 - 1;
				//int nextX = getCoordX() - 1;
				GlobalMap::Tiles nextTile = map.getGroundCellType(nextX, getCoordY());
				if ( nextTile == GlobalMap::Tiles::EMPTY || nextTile == GlobalMap::Tiles::WATER) {
					if (this->x - distanse < nextX + 1) {
						this->x = nextX + 1;
					}
				} else {
					this->x -= distanse;
				}
			}
			if (movingRight) {
				//int nextX = getCoordX() + getWidth() / 2;
				//GlobalMap::Tiles nextTile = map.getGroundCellType(getCoordX() + getWidth() / 2, getCoordY());
				int nextX = getCoordX() + getWidth() / 3.5;
				GlobalMap::Tiles nextTile = map.getGroundCellType(getCoordX() + getWidth() / 3.5, getCoordY());
				if ( nextTile == GlobalMap::Tiles::EMPTY || nextTile == GlobalMap::Tiles::WATER) {
					//if (this->x + distanse > nextX - 1) {
						//this->x = nextX - 1;
					//}
					if (this->x + distanse > nextX) {
						this->x = nextX;
					}
				} else {
					this->x += distanse;
				}
			}
		//}

	}
}

void Hero::setStairState(const MapManager & map) {
	GlobalMap::Tiles nextTile;
	if (movingUp) {
		nextTile = map.getGroundCellType(getCoordX(), getCoordY() - 1);
	} else if (movingDown) {
		nextTile = map.getGroundCellType(getCoordX(), getCoordY());
	}

	if (nextTile == GlobalMap::Tiles::STAIR) {
		if (movingUp) {
			nextStair = getCoordY() - Global::tileSize;
			this->state = State::STAIR_UP;
		} else if (movingDown) {
			nextStair = getCoordY() + Global::tileSize;
			this->state = State::STAIR_DOWN;
		}
	} else if (movingLeft || movingRight) {
		this->state = State::GO;
	}else {
		this->state = State::STAY;
	}

}

void Hero::save() {
	Db::setHeroCoord(getCoordX(), getCoordY(), isLookingRight);
}
