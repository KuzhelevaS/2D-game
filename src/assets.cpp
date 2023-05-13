#include "assets.h"

namespace asset {

	extern const std::vector<std::string> src = {"air.png","ground.png","hero.png","step.wav"};
	extern const std::map <std::string, std::string> srcList = {
		{"i_air", "air.png"},
		{"i_ground", "ground.png"},
		{"i_hero", "hero.png"},
		{"i_shadow", "hero.png"},
		{"s_step", "step.wav"},
		{"s_run", "run.wav"}
	};
}

