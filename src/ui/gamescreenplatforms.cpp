#include "gamescreenplatforms.h"
#include "resourcemanager.h"

GameScreenPlatforms::GameScreenPlatforms(sf::RenderWindow &window) :
	currentScreen(GameScreens::PLATFORMS), view(window.getView()), map(), hero(), flow() {

    Log::info("platforms counsructor");

	//groundImg.loadFromFile(map.getGroundImg());
	//groundImg.setSmooth(true);
	groundSprite.setTexture(ResourceManager::instance().useTexture(map.getGroundImg()));
	flow.addAnimation("FlightFlow", asset::srcList.at("i_air"), 0, 0, Global::tileSize, Global::tileSize, 1, 1, 4000);
	//flow.addAnimation("FlightFlow", "assets/air.png", 0, 0, Global::tileSize, Global::tileSize + 45, 1, 1, 4000);

	/*heroImg.loadFromFile(hero.getImg());
	heroImg.setSmooth(true);*/
	/*heroSprite.setTexture(ResourceManager::instance().useTexture(hero.getImg()));
	heroSprite.setOrigin(hero.getWidth() / 2, hero.getHeight());*/

	setViewOnHero();
}

GameScreenPlatforms::~GameScreenPlatforms() {
	Log::info("save");
	hero.save();
}

GameScreens GameScreenPlatforms::events(sf::Event &event, sf::RenderWindow &window) {
	if (event.type == sf::Event::KeyPressed) {
		switch (event.key.code) {
			case sf::Keyboard::I :
				Log::info("Open inventory");
				return GameScreens::BAG;
				break;
			case sf::Keyboard::A :
			case sf::Keyboard::Left :
				hero.setDirection(Hero::Direction::LEFT, true, map);
				break;
			case sf::Keyboard::D :
			case sf::Keyboard::Right :
				hero.setDirection(Hero::Direction::RIGHT, true, map);
				break;
			case sf::Keyboard::W :
			case sf::Keyboard::Up :
				hero.setDirection(Hero::Direction::UP, true, map);
				break;
			case sf::Keyboard::S :
			case sf::Keyboard::Down :
				hero.setDirection(Hero::Direction::DOWN, true, map);
				break;
			case sf::Keyboard::LShift :
				hero.switchRun(true);
				break;
			default:
				break;
		}
	}

	if (event.type == sf::Event::KeyReleased) {
		switch (event.key.code) {
			case sf::Keyboard::A :
			case sf::Keyboard::Left :
				hero.setDirection(Hero::Direction::LEFT, false, map);
				break;
			case sf::Keyboard::D :
			case sf::Keyboard::Right :
				hero.setDirection(Hero::Direction::RIGHT, false, map);
				break;
			case sf::Keyboard::W :
			case sf::Keyboard::Up :
				hero.setDirection(Hero::Direction::UP, false, map);
				break;
			case sf::Keyboard::S :
			case sf::Keyboard::Down :
				hero.setDirection(Hero::Direction::DOWN, false, map);
				break;
			case sf::Keyboard::LShift :
				hero.switchRun(false);
				break;
			default:
				break;
		}
	}

	if (event.type == sf::Event::Resized) {
		view.setSize(window.getSize().x, window.getSize().y);
		setViewOnHero();
	}

	return currentScreen;
}

GameScreens GameScreenPlatforms::otherEvents() {
    return currentScreen;
}

void GameScreenPlatforms::update(sf::Time deltaTime) {
	hero.update(deltaTime.asMilliseconds(), map);
	setViewOnHero();
	flow.update(deltaTime.asMilliseconds(), 0, 0);
}

void GameScreenPlatforms::render(sf::RenderWindow &window) {
	window.clear(sf::Color(220, 247, 248, 255));

	window.setView(view);

	drawGround(window);
	//Здания
	//растения/валуны и др. (кроме верхушек деревьев)
	//предметы
	drawHero(window);
	//Верхушка деревьев (на верхней клетке)
	//Доступные действия
	//Подсказки для действий
	//Интерфейс
}

void GameScreenPlatforms::setViewOnHero() {
    sf::Vector2f viewSize = view.getSize();
    int vWidth = viewSize.x;
    int vHeight = viewSize.y;
    int resultX = hero.getCoordX();
    int resultY = hero.getCoordY();
//    int mapW = Global::fullWidth * Global::tileSize;
//    int mapH = Global::fullHeight * Global::tileSize;
//    if (hero.getCoordX() < vWidth / 2 + Global::tileSize - Global::tileSize / 2) {
//		resultX = vWidth / 2 + Global::tileSize - Global::tileSize / 2;
//    }
//    if (hero.getCoordY() < vHeight / 2 - Global::tileSize / 2) {
//		resultY = vHeight / 2 - Global::tileSize / 2;
//    }
//    if (mapW - hero.getCoordX() < vWidth / 2 + Global::tileSize / 2) {
//		resultX = mapW - vWidth / 2 - Global::tileSize / 2;
//    }
//    if (mapH - Global::tileSize - hero.getCoordY() < vHeight / 2) {
//		resultY = mapH - Global::tileSize - vHeight / 2;
//    }
	view.setCenter(resultX, resultY);
}

void GameScreenPlatforms::drawGround(sf::RenderWindow &window) {
	sf::Sprite flowSprite;
	for (MapManager::groundCell &g : map.groundList) {
		if (g.type == GlobalMap::Tiles::STAIR ) {
			int offset = 0;//45;
			flow.setCoords(g.x, g.y - offset);
			flowSprite = flow.getSprite();
			if (map.getGroundCellType(g.x, g.y - Global::tileSize) == GlobalMap::Tiles::STAIR) {
				sf::IntRect rect = flowSprite.getTextureRect();
				rect.top += offset;
				rect.height -= offset;
				flowSprite.setTextureRect(rect);
				sf::Vector2f coords = flowSprite.getPosition();
				coords.y += offset;
				flowSprite.setPosition(coords);
			}
			window.draw(flowSprite);
		} else {
			sf::IntRect rect = {g.texture.x, g.texture.y, g.texture.w, g.texture.h};
			groundSprite.setTextureRect(rect);
			groundSprite.setPosition(g.x, g.y);
			window.draw(groundSprite);
		}
	}
}

void GameScreenPlatforms::drawHero(sf::RenderWindow &window) {
	/*sf::IntRect rect = {0, 0, hero.getWidth(), hero.getHeight()};
	heroSprite.setTextureRect(rect);
	heroSprite.setPosition(hero.getCoordX(), hero.getCoordY() + 12);
	window.draw(heroSprite);*/
	//window.draw(hero.picture.getCurrentFrame());
	//hero.picture.draw(window);


	//Тень
//	sf::Sprite shadow;
//	shadow.setTexture(ResourceManager::instance().useTexture(asset::srcList.at("i_hero")));
//	shadow.setOrigin(hero.getWidth()*2, 7);
//	sf::IntRect rect = {hero.getWidth() * 2, 0, hero.getWidth(), 14};
//	shadow.setPosition(hero.getCoordX() + hero.getWidth() * 1.5, hero.getCoordY() + 12);
//	GlobalMap::Tiles leftTile = map.getGroundCellType(hero.getCoordX() - hero.getWidth() / 2, hero.getCoordY());
//	if (leftTile == GlobalMap::Tiles::STAIR || leftTile == GlobalMap::Tiles::EMPTY) {
//		int space = Global::tileSize - (hero.getCoordX() - hero.getWidth() / 2) % Global::tileSize;
//		rect = {hero.getWidth() * 2 + space, 0, hero.getWidth() - space, 14};
//		shadow.setPosition(hero.getCoordX() + hero.getWidth() * 1.5 + space, hero.getCoordY() + 12);
//	}
//	GlobalMap::Tiles rightTile = map.getGroundCellType(hero.getCoordX() + hero.getWidth() / 2, hero.getCoordY());
//	if (rightTile == GlobalMap::Tiles::STAIR || rightTile == GlobalMap::Tiles::EMPTY || rightTile == GlobalMap::Tiles::WATER) {
//		int space = (hero.getCoordX() + hero.getWidth() / 2) % Global::tileSize;
//		rect = {hero.getWidth() * 2, 0, hero.getWidth() - space, 14};
//		shadow.setPosition(hero.getCoordX() + hero.getWidth() * 1.5, hero.getCoordY() + 12);
//	}
//	shadow.setTextureRect(rect);
//	if ( ! ((leftTile == GlobalMap::Tiles::STAIR || leftTile == GlobalMap::Tiles::EMPTY || leftTile == GlobalMap::Tiles::WATER)
//			&& (rightTile == GlobalMap::Tiles::STAIR || rightTile == GlobalMap::Tiles::EMPTY || rightTile == GlobalMap::Tiles::WATER))
//			&& hero.getCoordY() % Global::tileSize <= Global::tileSize / 2 - 7) {
//		window.draw(shadow);
//	}

	//Анимированный персонаж
	window.draw(hero.picture.getSprite());
}
