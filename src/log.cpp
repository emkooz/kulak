#include "log.hpp"

namespace kk
{
	std::vector<logBasic> logs;
	bool printLogOutput = true;

	void log (const std::string& msg)
	{
		logBasic bLog = {msg, normal};
		logs.push_back(bLog);

		if (printLogOutput)
			std::cout << std::endl << bLog.msg << " (" << bLog.type << ")" << std::flush;
	}

	void log (const std::string& msg, logType type)
	{
		logBasic bLog = {msg, type};
		logs.push_back(bLog);

		if (printLogOutput)
			std::cout << std::endl << bLog.msg << " (" << bLog.type << ")" << std::flush;
	}

	void log (logBasic bLog)
	{
		logs.push_back(bLog);

		if (printLogOutput)
			std::cout << std::endl << bLog.msg << " (" << bLog.type << ")" << std::flush;
	}
}