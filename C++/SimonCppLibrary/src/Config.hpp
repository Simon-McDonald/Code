/*
 * Config.h
 *
 *  Created on: Jan 27, 2016
 *      Author: Simon
 */

#ifndef SRC_CONFIG_HPP_
#define SRC_CONFIG_HPP_

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <vector>

namespace cvt {
	/*
	 * Convert string to given DataType. Return false on failure.
	 */
	template<typename DataType>
	bool fromStr(const std::string item, DataType &value) noexcept {
		std::istringstream streamConverter(item);

		streamConverter >> value;

		if (streamConverter.bad()) {
			return false;
		}

		return true;
	}

	template<>
	inline bool fromStr<std::string>(const std::string item, std::string &value) noexcept {
		value = item;
		return true;
	}

	/*
	 * Convert string to given DataType. Throws invalid_argument exception on failure.
	 */
	template<typename DataType>
	DataType fromStr(const std::string item) {
		DataType returnVal;
		bool result = fromStr(item, returnVal);

		if (!result) {
			std::string errorMsg = "Unable to convert '" + item + "' to typeid " + typeid(DataType).name();
			throw std::invalid_argument(errorMsg);
		}

		return returnVal;
	}

	template<>
	inline std::string fromStr<std::string>(const std::string item) {
		return item;
	}

	/*
	 * Convert given DataType to string. Throws invalid_argument exception on failure.
	 */
	template<typename DataType>
	DataType toStr(DataType &value) {
		std::stringstream streamConverter;
		streamConverter << value;

		if (streamConverter.bad()) {
			// Probably can't output value anyway.
			std::string errorMsg = "Unable to convert item of typeid " + std::string(typeid(DataType).name()) + ", value.";
			throw std::invalid_argument(errorMsg);
		}

		return static_cast<std::ostringstream*>(&(std::ostringstream() << value))->str();
	}

	template<>
	inline std::string toStr<std::string>(std::string &value) {
		return value;
	}

	inline std::string trim(std::string text) {
		if (!text.size()) {
			return "";
		}

		size_t startIdx = 0;
		while (startIdx < text.size() && isspace(text.at(startIdx))) {
			++startIdx;
		}

		size_t endIdx = text.size() - 1;
		while ((endIdx > startIdx) && (isspace(text.at(endIdx)))) {
			--endIdx;
		}

		if (startIdx > endIdx) {
			return "";
		} else {
			return text.substr(startIdx, endIdx - startIdx + 1);
		}
	}

	/*
	 * Split string by a delimiter.
	 */
	inline bool splitAtDelim(std::string &input, char delim, std::string &lhs, std::string &rhs) {
		size_t delimIndex = input.find(delim); // split at first delim

		if (delimIndex >= input.length()) {
			return false;
		}

		// strip spaces
		size_t startLhs = input.find_first_not_of(" ");
		size_t endLhs = input.find_last_not_of(" ", delimIndex - 1);

		if ((delimIndex == 0) || (startLhs == delimIndex)) {
			lhs = "";
		} else {
			lhs = input.substr(startLhs, endLhs - startLhs + 1);
		}

		size_t startRhs = input.find_first_not_of(" ", delimIndex + 1);
		size_t endRhs = input.find_last_not_of(" ");

		if ((startRhs >= input.size()) || (endRhs == delimIndex)) {
			rhs = "";
		} else {
			rhs = input.substr(startRhs, endRhs - startRhs + 1);
		}

		return true;
	}

	/*
	 * Convert given DataType to string. Throws invalid_argument exception on failure.
	 */
	template<typename DataType>
	std::vector<DataType> splitToList(std::string delimSeparatedList, char delim = ',') {
		std::vector<DataType> returnList;

		std::istringstream iss(delimSeparatedList);
		for (std::string str; std::getline(iss, str, delim);) {
			returnList.push_back(fromStr<DataType>(trim(str)));
		}

		return returnList;
	}

	/*
	 * Split line of format <key>[keyDelim]<val1>[listDelim]<val2>[listDelim]...
	 */
	template<typename DataType>
	std::vector<DataType> splitKeyList(std::string textLine, std::string keyValue,
			char keyDelim = '=', char listDelim = ',') {
		std::string textList;

		if (!splitAtDelim(textLine, keyDelim, keyValue, textList)) {
			return {};
		}

		return splitToList<DataType>(textList, listDelim);
	}
}

class Config {
public:
	typedef std::string ConfigKey, ConfigHeader;

	typedef std::map<ConfigKey, std::string> ConfigMap;
	typedef ConfigMap::value_type KeyValuePair;

	Config(std::string configFilePath) {
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
			// delimIndex will be trimmedLine.length() if no '=' in line.

			if (delimIndex < (trimmedLine.length() - 1) &&
				delimIndex != 0) {

				ConfigKey key = trimmedLine.substr(0, trimmedLine.find_last_not_of(" ", delimIndex - 1) + 1);

				std::string value = trimmedLine.substr(trimmedLine.find_first_not_of(" ", delimIndex + 1));

				this->addKey(configHeader, key, value);
			} else {
				// TODO ERROR File line unable to be registered.
			}
		}

		configFile.close();
	} /* Config::Config */

	template<typename DataType>
	bool addKey(ConfigHeader header, ConfigKey key, DataType value) {
		if (!header.length() || !key.length() || !value.length()) {
			std::cout << "Invalid values added to config map!" << std::endl;
			return false;
		}

		if (!this->configMap.count(header)) {
			this->configMap.emplace(header, ConfigMap());
		}

		ConfigMap &map = this->configMap.at(header);

		if (map.count(key)) {
			std::string errorMsg = "Config '" + header + ":" + key +
					"' duplicated. Initial value '" + map.at(key) +
					"' kept, '" + cvt::toStr(value) + "' discarded";
			throw std::invalid_argument(errorMsg);
		}

		map.emplace(key, cvt::toStr(value));
		return true;
	} /* Config::addKey */

	bool containsHeader(ConfigHeader header) {
		return this->configMap.count(header);
	} /* Config::containsHeader */

	bool containsKey(ConfigHeader header, ConfigKey key) {
		if (!this->containsHeader(header)) {
			std::string errorMessage = "Non-existant header '" + header + "'";
			throw std::invalid_argument(errorMessage);
		}

		return this->configMap.at(header).count(key);
	} /* Config::containsKey */

	void displayConfig(std::ostream &os) {
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
	} /* Config::displayConfig */

	void displayConfig(void) {
		this->displayConfig(std::cout);
	} /* Config::displayConfig */

	template<typename DataType>
	DataType getValue(ConfigHeader header, ConfigKey key) {
		std::string item = this->getString(header, key);
		return cvt::fromStr<DataType>(item);
	}

	template<typename DataType>
	DataType getValue(ConfigHeader header, ConfigKey key, DataType defaultValue) {
		std::string item;
		bool fetchResult = this->getString(header, key, item);

		if (!fetchResult) {
			return defaultValue;
		}

		DataType value;
		bool convertResult = cvt::fromStr<DataType>(item, value);

		if (!convertResult) {
			return defaultValue;
		}

		return value;
	}

	template<typename DataType>
	std::vector<DataType> getList(ConfigHeader header, ConfigKey key, char delim = ',') {
		std::vector<DataType> returnList;

		std::string item = this->getString(header, key);

		std::istringstream itemStream;
		itemStream.str(item);
		std::string entry;
		while (std::getline(itemStream, entry, delim)) {
			DataType value = cvt::fromStr<DataType>(entry);
			returnList.push_back(value);
		}

		return returnList;
	}

	template<typename DataType>
	bool getList(ConfigHeader header, ConfigKey key, std::vector<DataType> &returnList, char delim = ',') noexcept {
		std::string item;
		bool fetchResult = this->getString(header, key, item);
		if (!fetchResult) {
			return false;
		}

		std::istringstream itemStream;
		itemStream.str(item);
		std::string entry;
		bool convertResult = true;
		while (std::getline(itemStream, entry, delim)) {
			DataType value;
			convertResult &= cvt::fromStr<DataType>(header, key, entry);
			returnList.push_back(value);
		}
		return convertResult;
	}

private:
	typedef std::map<ConfigHeader, ConfigMap> ConfigHeaderMap;

	ConfigHeaderMap configMap;

	std::string getString(ConfigHeader header, ConfigKey key) {
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
	} /* Config::getString */

	bool getString(ConfigHeader header, ConfigKey key, std::string &value) {
		if (!this->containsHeader (header)) {
			return false;
		}

		if (!this->containsKey (header, key)) {
			return false;
		}

		value = this->configMap.at (header).at (key);
		return true;
	} /* Config::getStringEmpty */
};



/*
 * Read configuration information from file.
 */
/*class Config {
public:
	typedef std::string ConfigKey, ConfigHeader;

	typedef std::map<ConfigKey, std::string> ConfigMap;
	typedef ConfigMap::value_type KeyValuePair;

	Config(std::string);

	bool containsHeader(ConfigHeader);
	bool containsKey(ConfigHeader, ConfigKey);

	bool addKey(ConfigHeader, ConfigKey, std::string);

	std::string getString(ConfigHeader, ConfigKey);
	std::string getStringEmpty(ConfigHeader, ConfigKey);
	bool getBool(ConfigHeader, ConfigKey);
	char getChar(ConfigHeader, ConfigKey);
	unsigned char getUnsignedChar(ConfigHeader, ConfigKey);
	int getInt(ConfigHeader, ConfigKey);
	unsigned getUInt(ConfigHeader, ConfigKey);
	float getFloat(ConfigHeader, ConfigKey);
	void getVector(ConfigHeader, ConfigKey, std::vector<std::string>&);

	void displayConfig(void);
	void displayConfig(std::ostream&);
	void listConfigKeys(ConfigHeader, std::vector<ConfigKey>);
	void listConfigKeyValues(ConfigHeader, std::vector<KeyValuePair>);

	template<typename DataType>
	DataType getValue(ConfigHeader, ConfigKey);

	template<typename DataType>
	DataType getValue(ConfigHeader, ConfigKey, DataType);

private:
	typedef std::map<ConfigHeader, ConfigMap> ConfigHeaderMap;

	ConfigHeaderMap configMap;
};*/

#endif /* SRC_CONFIG_HPP_ */
