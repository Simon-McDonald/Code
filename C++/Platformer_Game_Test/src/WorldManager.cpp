/*
 * WorldManager.cpp
 *
 *  Created on: Mar 28, 2016
 *      Author: Simon
 */

#include <assert.h>

#include "WorldManager.h"

OpenGLWindow *WorldManager::window = nullptr;
std::map<std::string, ShaderManager> *WorldManager::shaders = nullptr;

WorldManager::WorldManager(void) {
} /* WorldManager::WorldManager */

WorldManager::~WorldManager(void) {
} /* WorldManager::~WorldManager */

void WorldManager::Initialise(OpenGLWindow *window, std::map<std::string, ShaderManager> *shaders) {
	if (window && !WorldManager::window) {
		WorldManager::window = window;
	}

	if (shaders && !WorldManager::shaders) {
		WorldManager::shaders = shaders;
	}
} /* WorldManager::Initialise */

OpenGLWindow& WorldManager::getWindow(void) {
	assert(WorldManager::window);

	return *WorldManager::window;
} /* WorldManager::getWindow */

ShaderManager& WorldManager::getShaderManager(std::string shaderName) {
	assert(WorldManager::shaders);

	return WorldManager::shaders->at(shaderName);
} /* WorldManager::getShaderManager */

ShaderManager& WorldManager::getShaderManager(void) {
	assert(WorldManager::shaders);

	return *WorldManager::shaders->begin()->second.getActiveShaderManager();
} /* WorldManager::getShaderManager */
