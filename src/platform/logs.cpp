#include "logs.h"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>

void LogManager::init(std::string name)
{

	this->name = name;
	firstLogAlreadyPlaced = 0;

}

void LogManager::log(const char *l, int type)
{

#if DEVELOPLEMT_BUILD == 1
	logInternally(l, type);
	logToFile(l, type);
	logToConsole(l, type);
#endif

#if PRODUCTION_BUILD == 1
	logToFile(l, type);
	logInternally(l, type);
#endif

}

std::stringstream formatLog(const char *l, int type)
{
	auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::stringstream s;
	s << "#" << std::put_time(std::localtime(&time), "%Y-%m-%d %X");

	if (type == LogManager::logWarning)
	{
		s << "[warning]";
	}
	else if (type == LogManager::logError)
	{
		s << "[error]";
	}

	s << ": ";
	s << l << "\n";
	return s;
}

void LogManager::logToFile(const char *l, int type)
{

	if (name == "") { name = DefaultLogFile; }

	//todo unlickely atribute
	if (!firstLogAlreadyPlaced)
	{
		firstLogAlreadyPlaced = 1;
		std::ofstream file(name);	//no need to check here
		file.close(); //clear the file content
	}

	::logToFile(name.c_str(), l, type);
}

void LogManager::logInternally(const char *l, int type)
{
	if (internalLogs.size() >= maxInternalLogCount)
	{
		internalLogs.pop_front();
	}

	internalLogs.push_back(formatLog(l, type).str());
}

void LogManager::logToConsole(const char *l, int type)
{
	std::cout << formatLog(l, type).str();
}


void logToFile(const char *fileName, const char *l, int type)
{
	std::ofstream file(fileName, std::ofstream::app);
	if (!file.is_open()) { return; }

	file << formatLog(l, type).rdbuf();
	file.close();
}

LogManager logsManager;

void platform::log(const char *l, int type)
{
	logsManager.log(l, type);
}

LogManager platform::getLogsManager()
{
	return logsManager;
}
