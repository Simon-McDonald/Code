/*
 * UtilityManager.h
 *
 *  Created on: Mar 27, 2016
 *      Author: Simon
 */

#ifndef UTILITYMANAGER_H_
#define UTILITYMANAGER_H_

#include <Config.h>
#include <Logger.h>

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

#endif /* UTILITYMANAGER_H_ */
