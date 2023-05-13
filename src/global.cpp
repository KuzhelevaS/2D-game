#include "global.h"
#include <random>
#include <ctime>

#if defined(DEBUG) || defined(RELEASE)
    std::mt19937 mersenne(static_cast<unsigned int>(time(0)));
#else
   std::random_device rd;
    std::mt19937 mersenne(rd());
#endif

int Global::intRand(int min, int max) {
	return min + mersenne() % (max - min + 1);
}
