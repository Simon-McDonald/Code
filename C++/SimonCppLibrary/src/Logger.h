/*
 * Logger.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Simon
 */

#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_

#include <iostream>
#include <stdarg.h>
#include <iomanip>
#include <string>
#include <string.h>

#define LOG_ARGS __FILE__, __func__, __LINE__

/* std::setfill('0') << std::setw(5) << */


#define OUTPUT(logStatus) \
	std::cout << std::setfill(' ') \
	<< std::setw(40) << std::left << __FILE__ << std::setw(0) << ": "  \
	<< std::setw(25) << std::left << __func__ << std::setw(0) << ": " \
	<< std::setw(5) << std::right << __LINE__ << std::setw(0) << ": " \
	<< std::setw(6) << std::left << #logStatus << std::setw(0) << ": "

#define INFO OUTPUT(INFO)
#define DEBUG OUTPUT(DEBUG)
#define WARN OUTPUT(DEBUG)
#define ERR OUTPUT(ERROR)

class Logger {
public:
	enum LogLevel {
		info,
		debug,
		warn,
		error,
		numLevels
	};

	Logger();

	void Log(LogLevel level, std::string formatString, ...);
	void Log(LogLevel level, std::string fileName, std::string funcName,
	int lineNum, std::string formatString, ...);

	static std::string formatLength(std::string baseString, size_t length);

	~Logger();

private:
	static const char levelNames[numLevels][8];
	static const int fileNameLength;
	static const int funcNameLength;
	static const int lineNumLength;
	static const int logLevelLength;
};

#endif /* SRC_LOGGER_H_ */
