/*
 * UtilityManager.cpp
 *
 *  Created on: Mar 27, 2016
 *      Author: Simon
 */

#include <assert.h>

#include "UtilityManager.h"

Config *UtilityManager::config = nullptr;
Logger *UtilityManager::logger = nullptr;

UtilityManager::UtilityManager(void) {
} /* UtilityManager::UtilityManager */

UtilityManager::~UtilityManager(void) {
} /* UtilityManager::UtilityManager */

void UtilityManager::Initialise(Config *config, Logger *logger) {
	if (config && !UtilityManager::config) {
		UtilityManager::config = config;
	}

	if (logger && !UtilityManager::logger) {
		UtilityManager::logger = logger;
	}
} /* UtilityManager::Initialise */

Config& UtilityManager::getConfig(void) {
	assert (UtilityManager::config);

	return *UtilityManager::config;
} /* UtilityManager::getConfig */

Logger& UtilityManager::getLogger(void) {
	assert (UtilityManager::logger);

	return *UtilityManager::logger;
} /* UtilityManager::getLogger */
