#pragma once
#include <vector>
#include <string>
#include <iostream>

/*
Usage:
	kk::log("log 1, this is a test");
	kk::log("log 2, warning log", static_cast<kk::logType>(1));
	kk::log(kk::logBasic{"log 3, error", kk::logType::error});
*/

namespace kk
{
	enum logType {normal, warning, error};

	struct logBasic
	{
		std::string msg;
		logType type;
	};

	extern std::vector<logBasic> logs;
	extern bool printLogOutput;

	void log (const std::string& msg);
	void log (const std::string& msg, logType type);
	void log (logBasic _log);
}