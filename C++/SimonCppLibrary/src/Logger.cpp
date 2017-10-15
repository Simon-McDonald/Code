/*
 * Logger.cpp
 *
 *  Created on: Mar 26, 2016
 *      Author: Simon
 */

#include "Logger.hpp"

LogLevel::LogLevel(Type t) : t_(t) {};

LogLevel::operator LogLevel::Type () const {
	return t_;
};

const std::string& LogLevel::toString () const {
	CREATE_LOCAL_LIST(test_list, LOG_LEVEL_ENUM_LIST)
	unsigned idx = (unsigned) this->t_;
	return test_list[idx];
};

LogLevel::operator const std::string& () const {
	return this->toString();
}

std::ostream& operator<<(std::ostream& os, const LogLevel& level) {
	os << level.toString(); return os;
}

std::ostream& operator<<(std::ostream& os, const LogLevel::Type& type) {
	os << LogLevel(type).toString(); return os;
}


const unsigned Logger::registerLogLevel(std::string newLogLevel) {
	unsigned newId = customLogLevels.size();
	customLogLevels.push_back(newLogLevel);
	return newId;
}

Logger::Logger() : customLogLevels({"UNDEFINED"}), currentStream(nullptr) {}

std::ostream& Logger::Log(std::ostream &os, std::string levelString, const char *fileName, const char *functionName, int lineNum) {
	os << std::setfill(' ')
		<< std::setw(40) << std::left << fileName <<
		std::setw(0) << ": " <<
		std::setw(25) << std::left << functionName <<
		std::setw(0) << ": " <<
		std::setw(5) << std::right << lineNum <<
		std::setw(0) << ": " <<
		std::setw(6) << std::left << levelString <<
		std::setw(0) << ": ";
	return os;
}

std::ostream& Logger::Log(LogLevel::Type level, const char *fileName, const char *functionName, int lineNum) {
	if (this->currentStream) {
		//*this->currentStream << '\n';
	}

	this->currentStream = &std::cout;
	return this->Log(*this->currentStream, LogLevel(level), fileName, functionName, lineNum);
}

std::ostream& Logger::Log(unsigned level, const char *fileName, const char *functionName, int lineNum) {
	if (this->currentStream) {
		//*this->currentStream << '\n';
	}

	this->currentStream = &std::cout;
	return this->Log(*this->currentStream, this->customLogLevels.at(level), fileName, functionName, lineNum);
}

std::ostream& Logger::Log() {
	if (!this->currentStream) {
		return std::cout;
	}

	return *this->currentStream;
}

std::ostream& Logger::Flush() {
	if (!this->currentStream) {
		return std::cout;
	}

	//return this->currentStream->flush();
	//return *this->currentStream;// << std::endl;
	return std::cout << "()";
	//return std::endl;
}

