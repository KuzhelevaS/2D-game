#include "resourcemanager.h"
#include <fstream>
#include <iostream>


ResourceManager::ResourceManager()
{
	//ctor
}

ResourceManager & ResourceManager::instance() {
	static ResourceManager * instance = new ResourceManager();
	return * instance;
}

sf::Texture& ResourceManager::useTexture(const std::string & fileName) {
    if(textures.count(fileName) == 0){
		sf::Texture tex;
		std::vector<char> buffer = getBufferFile(fileName);
		tex.loadFromMemory(&buffer[0], buffer.size());
		//tex.loadFromFile(fileName);
		tex.setSmooth(true);
		this->textures[fileName] = tex;
    }
    return textures.at(fileName);
}

sf::SoundBuffer& ResourceManager::useSound(const std::string & fileName) {
    if(sounds.count(fileName) == 0){
		sf::SoundBuffer sound;
		std::vector<char> buffer = getBufferFile(fileName);
		sound.loadFromMemory(&buffer[0], buffer.size());
		//sound.loadFromFile(fileName);
		this->sounds[fileName] = sound;

    }
    return sounds.at(fileName);
}

std::vector<char> ResourceManager::getBufferFile(const std::string & fileName) {
	//std::ifstream texture_file{fileName, std::ifstream::binary};
	//std::vector<char> buffer;

	std::ifstream read("assets/assets.dat", std::ios::binary);

	std::vector<char> buffer(std::istreambuf_iterator<char>(read), {});
    int numCount = 0;
    int maxHeader = 0;
	std::string numText = "";
    std::vector<int> fileEndArr;

    for (int i = 0; i < buffer.size(); i++) {
		//std::cout << buffer[i] << " ";
		if (buffer[i] == '/') {
			numCount++;
			fileEndArr.push_back(std::stoi( numText ));
			numText = "";
		} else {
			numText += buffer[i];
		}
		if(numCount == asset::src.size()) {
			maxHeader = i + 1;
			break;
		}
    }

    std::map <std::string, std::vector<char>> srcList;
    for (int i = 0; i < asset::src.size(); i++) {
		int firstCell = 0;
		if (i == 0) {
			firstCell = maxHeader;
		} else {
			firstCell = fileEndArr[i-1] + maxHeader;
		}
		int lastCell = buffer.size();
		if (i < asset::src.size() - 1) {
			lastCell = fileEndArr[i] + maxHeader;
		}

		//std::cout << firstCell << "/" << lastCell << std::endl;
		std::vector<char> currentBuffer;
		for (int j = firstCell; j < lastCell; j++) {
			currentBuffer.push_back(buffer[j]);
		}
		srcList.insert(std::make_pair(asset::src[i], currentBuffer));
	}

	std::vector<char> result;
	for (const auto& [key, value] : srcList) {
		std::string keyName = key;
		//std::string keyName = "assets/" + key;
		//std::cout << keyName << " / " << fileName << '\n' ;
		if (keyName == fileName) {
			//std::cout << key << " has value " << '\n' ;
			for (int j = 0; j < value.size(); j++) {
				result.push_back(value[j]);
				//std::cout << value[j];
			}
			//std::cout << std::endl;
		}
	}
//	if (texture_file) {
//		// get length of file:
//		texture_file.seekg(0, texture_file.end);
//		const auto length = texture_file.tellg();
//		if (length) {
//			buffer.resize(length); // reserve space
//
//			texture_file.seekg(0, texture_file.beg);
//
//			auto start = &*buffer.begin();
//			texture_file.read(start, length);
//		}
//
//		texture_file.close();
//	}

	return result;
}
