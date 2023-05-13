#include "log.h"

enum class States {
	ERROR,
	INFO
};

void logTpl(States status, const std::string &message) {
	std::string fileName = "starsurvival.log";
	std::fstream logFile(fileName, std::fstream::in | std::fstream::out | std::fstream::app);
	if (logFile.is_open()) {
		time_t now = time(nullptr);
		tm * dateTime = localtime(&now);

		logFile << 1900 + dateTime->tm_year << "/";

		if ((dateTime->tm_mon + 1) < 10) {
			logFile << 0;
		}
		logFile << 1 + dateTime->tm_mon << "/";

		if (dateTime->tm_mday < 10) {
			logFile << 0;
		}
		logFile << dateTime->tm_mday << " ";

		if (dateTime->tm_hour < 10) {
			logFile << 0;
		}
		logFile << dateTime->tm_hour << ":";

		if (dateTime->tm_min < 10) {
			logFile << 0;
		}
		logFile << dateTime->tm_min << ":";

		if (dateTime->tm_sec < 10) {
			logFile << 0;
		}
		logFile << dateTime->tm_sec << " - ";

		switch (status) {
			case States::ERROR:
				logFile << "ERR" << " - ";
				break;
			case States::INFO:
				logFile << "INF" << " - ";
				break;
			default:
				break;
		}

		logFile << message << std::endl;
	}
	logFile.close();
}

void Log::error(const std::string & message) {
	logTpl(States::ERROR, message);
	std::cout << message << std::endl;
}

void Log::info(const std::string & message) {
	logTpl(States::INFO, message);
	std::cout << message << std::endl;
}

