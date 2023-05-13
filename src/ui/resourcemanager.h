#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "assets.h"

class ResourceManager
{
	public:
		static ResourceManager & instance();

		sf::Texture& useTexture(const std::string & fileName);
		sf::SoundBuffer& useSound(const std::string & fileName);

	protected:

	private:
		ResourceManager();
		std::map<std::string, sf::Texture> textures;
		std::map<std::string, sf::SoundBuffer> sounds;
		std::vector<char> getBufferFile(const std::string & fileName);

};

#endif // RESOURCEMANAGER_H
