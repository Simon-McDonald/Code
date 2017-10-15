/*
 * UtilityManager.h
 *
 *  Created on: Mar 27, 2016
 *      Author: Simon
 */

#ifndef UTILITYMANAGER_HPP_
#define UTILITYMANAGER_HPP_

#include <string>

#include <Config.hpp>
#include <Logger.hpp>

class UtilityManager {
public:
	static void Initialise(Config *config, Logger *logger);

	~UtilityManager(void);

private:
	static Config *config;
	static Logger *logger;

protected:
	UtilityManager(void);

	static Config& getConfig(void);
	static Logger& getLogger(void);
};

#endif /* UTILITYMANAGER_HPP_ */
