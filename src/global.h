#ifndef GLOBAL_H
#define GLOBAL_H

namespace Global {
	inline constexpr int tileSize = 180;

	inline constexpr int areaWidth = 120;//120;12
	inline constexpr int areaHeight = 30;//30;12
	inline constexpr int countAreaX = 1;
	inline constexpr int countAreaY = 1;
	inline constexpr int fullWidth = areaWidth * countAreaX;
	inline constexpr int fullHeight = areaHeight * countAreaY;

	inline constexpr int platformMinW = 4;
	inline constexpr int platformMinH = 1;

	int intRand(int min, int max);
};

#endif // GLOBAL_H
