/*
 * Logger.cpp
 *
 *  Created on: Mar 26, 2016
 *      Author: Simon
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "Logger.h"

const char Logger::levelNames[Logger::numLevels][8] = {"INFO", "DEBUG", "WARN", "ERROR"};
const int Logger::fileNameLength = 25;
const int Logger::funcNameLength = 12;
const int Logger::lineNumLength = 4;
const int Logger::logLevelLength = 6;

Logger::Logger() {
} /* Logger::Logger */

void Logger::Log(Logger::LogLevel level, std::string formatString, ...)  {
	char formatBuffer[256];

	sprintf(formatBuffer, "%.*s",
			this->fileNameLength + this->funcNameLength + this->lineNumLength + 3,
			" ");

	sprintf(formatBuffer + strlen(formatBuffer), "%.*s:",
			this->logLevelLength, levelNames[level]);

	va_list argList;
	va_start (argList, formatString);

	vsprintf(formatBuffer + strlen(formatBuffer), formatString.c_str(), argList);
	va_end (argList);

	printf ("%s\n", formatBuffer);
}; /* Logger::Log */

void Logger::Log(LogLevel level, std::string fileName, std::string funcName,
	int lineNum, std::string formatString, ...) {
	char formatBuffer[256];

	sprintf(formatBuffer, "%.*s:%.*s:%.*d:",
			this->fileNameLength, fileName.c_str(),
			this->funcNameLength, funcName.c_str(),
			this->lineNumLength, lineNum);

	sprintf(formatBuffer + strlen(formatBuffer), "%.*s:",
			this->logLevelLength, levelNames[level]);

	va_list argList;
	va_start(argList, formatString);

	vsprintf(formatBuffer + strlen(formatBuffer), formatString.c_str(), argList);
	va_end(argList);

	printf ("%s\n", formatBuffer);
} /* Logger::Log */

Logger::~Logger() {
} /* Logger::~Logger */
