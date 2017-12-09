/*
 * Config.h
 *
 *  Created on: Jan 27, 2016
 *      Author: Simon
 */

#ifndef SRC_CONFIG_HPP_
#define SRC_CONFIG_HPP_

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

/*
 * Read configuration information from file.
 */
class Config {
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
};

#endif /* SRC_CONFIG_HPP_ */
