#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "src/assets.h"

int main()
{
	//Шаг 1 - склеиваем файлы
	asset::src;
	std::ofstream out("assets/assets.dat", std::ios::binary | std::ios::out);

	int fileEnd = 0;
	for (int i = 0; i < asset::src.size(); i++) {
		std::string assetPath = "assets/src/" + asset::src[i];
		std::ifstream input(assetPath, std::ios::binary | std::ios::ate);
		fileEnd += input.tellg();
		input.close();
		out << fileEnd << '/';
		std::cout << assetPath << std::endl;
	}

	for (int i = 0; i < asset::src.size(); i++) {
		std::string assetPath = "assets/src/" + asset::src[i];
		std::ifstream input(assetPath, std::ios::binary);
		std::copy(
			std::istreambuf_iterator<char>(input),
			std::istreambuf_iterator<char>( ),
			std::ostreambuf_iterator<char>(out));
		input.close();
	}
//    std::ifstream text1("test1.txt", std::ios::binary);
//    std::ifstream text2("test2.txt", std::ios::binary);
//    std::ifstream img("icon.png", std::ios::binary);


    //out << text1.rdbuf() << text2.rdbuf();
//    std::copy(
//        std::istreambuf_iterator<char>(text1),
//        std::istreambuf_iterator<char>( ),
//        std::ostreambuf_iterator<char>(out));
//	std::copy(
//        std::istreambuf_iterator<char>(text2),
//        std::istreambuf_iterator<char>( ),
//        std::ostreambuf_iterator<char>(out));
//	std::copy(
//        std::istreambuf_iterator<char>(img),
//        std::istreambuf_iterator<char>( ),
//        std::ostreambuf_iterator<char>(out));

//    text1.close();
//    text2.close();
//    img.close();
    out.close();

    //Шаг 2 - кодируем файл

    //Шаг 3 - декодируем буффер

	//Шаг 3 - читаем буффер
    std::ifstream read("assets/assets.dat", std::ios::binary);
    //std::cout << read.std::ios::rdbuf(std::cout.rdbuf());

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(read), {});
    int numCount = 0;
    int maxHeader = 0;
	std::string numText = "";
    std::vector<int> fileEndArr;

    //number = pow(256,0) * bytes[0] + pow(256,1) * bytes[1] + pow(256,2) * bytes[2] + pow(256,3) * bytes[3];
    //numVal += numVal * 10 + buffer[i];
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

    //Разбиваем файл на составляющие благодаря вектору с длинами
    std::map <std::string, std::vector<unsigned char>> srcList;
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

		std::cout << firstCell << "/" << lastCell << std::endl;
		std::vector<unsigned char> currentBuffer;
		for (int j = firstCell; j < lastCell; j++) {
			currentBuffer.push_back(buffer[j]);
		}
		srcList.insert(std::make_pair(asset::src[i], currentBuffer));
	}
    //srcList[fileBegin[i]] = num_2;

	for (const auto& [key, value] : srcList) {
		std::cout << key << " has value " << '\n' ;
		for (int j = 0; j < value.size(); j++) {
			std::cout << value[j];
		}
		std::cout << std::endl;
	}

    return 0;
}
