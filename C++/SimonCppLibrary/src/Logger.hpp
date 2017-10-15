/*
 * Logger.h
 *
 *  Created on: Jan 16, 2016
 *      Author: Simon
 */

#ifndef SRC_LOGGER_HPP_
#define SRC_LOGGER_HPP_

#include <iostream>
#include <stdarg.h>
#include <iomanip>
#include <string>
#include <vector>

#include "EnumMacro.h"

/*
 * The default logging levels that are offered by the class.
 */
#define LOG_LEVEL_ENUM_LIST(ELEM) \
	ELEM(Error), \
	ELEM(Warn), \
	ELEM(Info), \
	ELEM(Debug)

struct LogLevel {
public:
	CREATE_LOCAL_ENUM(Type, LOG_LEVEL_ENUM_LIST)

	/* Must always initialise with a parameter */
	LogLevel(Type t);

	/* Implicit Promotion to the enum type */
	operator Type () const;

	/* Conversions */
	const std::string& toString () const;
	operator const std::string& () const;
	friend std::ostream& operator<<(std::ostream& os, const LogLevel& level);
	friend std::ostream& operator<<(std::ostream& os, const Type& type);

private:
	const Type t_;

	/* Prevent default promotions */
	template<typename T>
	operator T () const;
};

inline std::ostream& finaliseLog (std::ostream& os) {
	os.put('\n');
	os.flush();
	return os;
}

/*
 * Require a get method in the class (getLogger) to use.
 */
#define LOG_ARGS __FILE__, __func__, __LINE__

#define ACCESSOR UtilityManager::getLogger()
#define OUTPUT(logStatus) ACCESSOR .Log(logStatus, LOG_ARGS)
#define APPEND ACCESSOR .Log()
//#define END ACCESSOR .Flush()
#define END finaliseLog

#define INFO OUTPUT(LogLevel::Info)
#define DEBUG OUTPUT(LogLevel::Debug)
#define WARN OUTPUT(LogLevel::Warn)
#define ERR OUTPUT(LogLevel::Error)
#define LOG(CUSTOM_LEVEL) OUTPUT(CUSTOM_LEVEL)

class Logger {
public:
	Logger();

	/*
	 * Use the returned value to log to the specific level that was registered.
	 */
	const unsigned registerLogLevel(std::string newLogLevel);

	std::ostream& Log(LogLevel::Type, const char *fileName, const char *functionName, int lineNum);
	std::ostream& Log(unsigned level, const char *fileName, const char *functionName, int lineNum);
	std::ostream& Log();

	std::ostream& Flush();

private:
	std::vector<std::string> customLogLevels;
	std::ostream *currentStream;

	std::ostream& Log(std::ostream &os, std::string levelString, const char *fileName, const char *functionName, int lineNum);
};

#endif /* SRC_LOGGER_HPP_ */
