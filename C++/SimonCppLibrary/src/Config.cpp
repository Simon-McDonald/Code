/*
 * Config.cpp
 *
 *  Created on: Jan 27, 2016
 *      Author: Simon
 */

/*#include "Config.hpp"

#include <assert.h>
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>

#include "Logger.hpp"

namespace bla {
	template<typename DataType>
	DataType convert(std::string item) {
		std::stringstream streamConverter(item);

		DataType value;
		streamConverter >> value;
		// check for error

		return value;
	}

	template<>
	std::string convert<std::string>(std::string item) {
		return item;
	}
}

Config::Config(std::string configFilePath) {
	std::ifstream configFile;
	configFile.open(configFilePath.c_str());

	if (!configFile.is_open()) {
		throw std::invalid_argument("Invalid configuration path");
	}

	ConfigHeader configHeader = "";

	while (!configFile.eof()) {
		std::string fileLine;
		getline(configFile, fileLine);

		if (!fileLine.length()) {
			continue;
		}

		size_t lineStart = fileLine.find_first_not_of(" ");
		size_t lineEnd = fileLine.find_last_not_of(" ");

		std::string trimmedLine = fileLine.substr(lineStart, lineEnd - lineStart + 1);

		if (trimmedLine.at(0) == '#') {
			continue;
		}

		if ((trimmedLine.at(0) == '[') && (trimmedLine.at(trimmedLine.length() - 1) == ']')) {
			configHeader = trimmedLine.substr(1, trimmedLine.length() - 2);
			continue;
		}

		size_t delimIndex = trimmedLine.find("=");

		if (delimIndex != trimmedLine.length()) {
			ConfigKey key = trimmedLine.substr(0, trimmedLine.find_last_not_of(" ", delimIndex));
			std::string value = trimmedLine.substr(trimmedLine.find_first_not_of(" ", delimIndex + 1));

			this->addKey(configHeader, key, value);
		}
	}

	configFile.close();
}

bool Config::addKey(ConfigHeader header, ConfigKey key, std::string value) {
	if (!header.length() || !key.length() || !value.length()) {
		std::cout << "Invalid values added to config map!" << std::endl;
		return false;
	}

	if (!this->configMap.count(header)) {
		this->configMap.emplace(header, ConfigMap());
	}

	ConfigMap &map = this->configMap.at(header);

	if (map.count(key)) {
		//WARN("Config '%s:%s' duplicated. Initial value '%s' kept, '%s' discarded");
		return false;
	}

	map.emplace(key, value);
	return true;
}

bool Config::containsHeader(ConfigHeader header) {
	return this->configMap.count(header);
}

bool Config::containsKey(ConfigHeader header, ConfigKey key) {
	if (!this->containsHeader(header)) {
		std::string errorMessage = "Non-existant header '" + header + "'";
		throw std::invalid_argument(errorMessage);
	}

	return this->configMap.at(header).count(key);
}

std::string Config::getString(ConfigHeader header, ConfigKey key) {
	if (!this->containsKey(header, key)) {
		std::string errorMessage = "Non-existant key '" + key + "' under header '" + header + "'";
		throw std::invalid_argument(errorMessage);
	}

	std::string value = this->configMap.at(header).at(key);

	if (!value.length ()) {
		std::string errorMessage = "Empty value for key '" + key + "' under header '" + header + "'";
		throw std::invalid_argument (errorMessage);
	}

	return value;
}

std::string Config::getStringEmpty(ConfigHeader header, ConfigKey key) {
	if (!this->containsHeader (header)) {
		return "";
	}

	if (!this->containsKey (header, key)) {
		return "";
	}

	return this->configMap.at (header).at (key);
}

bool Config::getBool(ConfigHeader header, ConfigKey key) {
	std::string value = this->getStringEmpty (header, key);

	if (!value.length()) {
		return false;
	}

	for (std::string::iterator str_itr = value.begin (); str_itr != value.end (); str_itr++) {
		if (isdigit (*str_itr)) {
			return *str_itr != '0';
		}

		if (isalpha (*str_itr)) {
			switch (tolower (*str_itr)) {
				case 'y':
				case 't':
					return true;
			}

			return false;
		}
	}

	return false;
}

char Config::getChar (ConfigHeader header, ConfigKey key) {
	std::string value = this->getString (header, key);

	if (!value.length()) {
		std::string errorMessage = "Empty value for key '" + key + "' under header '" + header + "'";
		throw std::invalid_argument (errorMessage);
	}

	return value.at(0);
}

unsigned char Config::getUnsignedChar(ConfigHeader header, ConfigKey key) {
	std::string value = this->getString (header, key);

	return this->getChar(header, key);
}

int Config::getInt(ConfigHeader header, ConfigKey key) {
	std::string value = this->getString(header, key);

	if (!value.length()) {
		std::string errorMessage = "Empty value for key '" + key + "' under header '" + header + "'";
		throw std::invalid_argument(errorMessage);
	}

	int intValue = atoi(value.c_str());

	return intValue;
}

unsigned Config::getUInt(ConfigHeader header, ConfigKey key) {
	std::string value = this->getString(header, key);

	if (!value.length()) {
		std::string errorMessage = "Empty value for key '" + key + "' under header '" + header + "'";
		throw std::invalid_argument(errorMessage);
	}

	unsigned intValue = stoul(value);

	return intValue;
}

float Config::getFloat(ConfigHeader header, ConfigKey key) {
	std::string value = this->getString(header, key);

	if (!value.length()) {
		std::string errorMessage = "Empty value for key '" + key + "' under header '" + header + "'";
		throw std::invalid_argument(errorMessage);
	}

	float floatValue = atof(value.c_str());

	return floatValue;
}

void Config::getVector(ConfigHeader header, ConfigKey key, std::vector<std::string> &valueList) {
	std::string value = this->getString(header, key);

	std::stringstream ss;
	ss.str(value);
	std::string item;
	while (std::getline(ss, item, ',')) {
		valueList.push_back(item);
	}
}

void Config::displayConfig(std::ostream &os) {
	os << "----Configuration----" << std::endl;

	for (ConfigHeaderMap::iterator mapItr = this->configMap.begin();
		 mapItr != this->configMap.end();
		 mapItr++) {

		os << "[" << mapItr->first << "]" << std::endl;
		ConfigMap &inMap = mapItr->second;

		for (ConfigMap::iterator inMapItr = inMap.begin();
			 inMapItr != inMap.end();
			 inMapItr++) {
			os << inMapItr->first << ":" << inMapItr->second << std::endl;
		}
	}
}

void Config::displayConfig(void) {
	this->displayConfig(std::cout);
}

void Config::listConfigKeys(ConfigHeader header, std::vector<ConfigKey> keyList) {
	if (!this->containsHeader(header)) {
		std::string errorMessage = "Non-existant header '" + header + "'";
		throw std::invalid_argument(errorMessage);
	}

	ConfigMap &mapItr = this->configMap.at(header);

	for (ConfigMap::iterator inMapItr = mapItr.begin();
		 inMapItr != mapItr.end();
		 inMapItr++) {
		keyList.push_back(inMapItr->first);
	}
}

void Config::listConfigKeyValues(ConfigHeader header, std::vector<KeyValuePair> keyValueList) {
	if (!this->containsHeader(header)) {
		std::string errorMessage = "Non-existant header '" + header + "'";
		throw std::invalid_argument(errorMessage);
	}

	ConfigMap &mapItr = this->configMap.at(header);

	for (ConfigMap::iterator inMapItr = mapItr.begin();
		 inMapItr != mapItr.end();
		 inMapItr++) {
		keyValueList.push_back(*inMapItr);
	}
}

template<typename DataType>
DataType Config::getValue(ConfigHeader header, ConfigKey key) {
	std::string item = this->getString(header, key);

	try {
		DataType value = bla::convert<DataType>(item);
		return value;
	} catch (...) {
		std::string errorMessage = "Bad conversion '" + header + "'";
		throw std::invalid_argument(errorMessage);
	}
}

template<typename DataType>
DataType Config::getValue(ConfigHeader header, ConfigKey key, DataType dataType) {
	std::string item = this->getStringEmpty(header, key);

	try {
		DataType value = bla::convert<DataType>(item);
		return value;
	} catch (...) {
		// some warning
		return dataType;
	}
}*/



