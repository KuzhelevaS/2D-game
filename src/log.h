#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <iostream>
#include <ctime>

namespace Log {
	void error(const std::string & message);
	void info(const std::string & message);
};

#endif //LOG_H
