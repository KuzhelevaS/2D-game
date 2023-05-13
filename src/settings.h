#ifndef SETTINGS_H
#define SETTINGS_H

#include "db.h"

enum class GameScreens {
	PLATFORMS,
	//BATTLE,
	BAG,
	//CRAFT,
	//TALK,
	//SKILL,
	//SHOP,
	//MAP,
	//HOUSE,
	MAX
};

class Settings
{
	public:
		static Settings & gs ();
		~Settings();

		GameScreens getGameScreen() const;
		void setGameScreen(GameScreens newScreen);

	protected:

	private:
		Settings();
};

#endif // SETTINGS_H
