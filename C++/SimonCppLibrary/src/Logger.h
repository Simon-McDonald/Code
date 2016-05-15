/*
 * Logger.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Simon
 */

#ifndef SRC_LOGGER_H_
#define SRC_LOGGER_H_

#include <stdarg.h>
#include <string>
#include <string.h>

#define LOG_ARGS __FILE__, __func__, __LINE__

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

	~Logger();

private:
	static const char levelNames[numLevels][8];
	static const int fileNameLength;
	static const int funcNameLength;
	static const int lineNumLength;
	static const int logLevelLength;
};

#endif /* SRC_LOGGER_H_ */
