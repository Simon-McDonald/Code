/*
 * SkyMapManager.cpp
 *
 *  Created on: Jul 2, 2016
 *      Author: Simon
 */

#include "SkyMapManager.h"

SkyMapManager::SkyMapManager(void) {}

SkyMapManager::SkyMapManager(std::string xPlusTexture, std::string xNegTexture,
							 std::string yPlusTexture, std::string yNegTexture,
							 std::string zPlusTexture, std::string zNegTexture) :
	textureFileNames({xPlusTexture, xNegTexture, yPlusTexture,
	yNegTexture, zPlusTexture, zNegTexture}), textureBufferID(-1u) {
}

bool SkyMapManager::initialiseSkyMap(void) {
	this->loadCubeTexture(this->textureFileNames, "SkyMap");

	DEBUG << "Loading the small cube mesh object" << std::endl;
	this->loadMesh("smallCube.obj");

	return true;
}

bool SkyMapManager::initialiseSkyMap(std::string configItem) {

}

void SkyMapManager::bindSkyMap(std::string samplerName) {
	this->setCubeTexture(samplerName, "SkyMap");
}

void SkyMapManager::renderSkyMap(void) {
	this->renderMesh("smallCube.obj");
}

SkyMapManager::~SkyMapManager(void) {
}
