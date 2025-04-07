#pragma once
#include <string>
#include <deque>

struct LogManager
{
	enum: int
	{
		logNormal = 0,
		logWarning,
		logError
	};

	LogManager() {};

	static constexpr const char *DefaultLogFile = RESOURCES_PATH "../logs.txt";

	//a null name will just log to a internal structure
	void init(std::string name);


	//this will be dependent on the configuration of the project. 
	void log(const char *l, int type = logNormal);


	std::string name = "";
	bool firstLogAlreadyPlaced = 0;

	static constexpr int maxInternalLogCount = 100;
	std::deque<std::string> internalLogs; //used to print the logs in a internal console if you want to implement that

	//used interally usually.
	void logToFile(const char *l, int type = logNormal);
	void logInternally(const char *l, int type = logNormal);
	void logToConsole(const char *l, int type = logNormal);

};

void logToFile(const char *fileName, const char *l, int type = LogManager::logNormal);


namespace platform
{

	void log(const char *l, int type = ::LogManager::logNormal);

	LogManager getLogsManager();
};