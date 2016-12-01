/*
 * SkyMapManager.cpp
 *
 *  Created on: Jul 2, 2016
 *      Author: Simon
 */

#include "SkyMapManager.h"

SkyMapManager::SkyMapManager(std::string xPlusTexture, std::string xNegTexture,
							 std::string yPlusTexture, std::string yNegTexture,
							 std::string zPlusTexture, std::string zNegTexture) :
	textureFileNames({xPlusTexture, xNegTexture, yPlusTexture,
	yNegTexture, zPlusTexture, zNegTexture}), textureBufferID(-1u) {
}
#include <stdio.h>
bool SkyMapManager::initialiseSkyMap(void) {

	this->loadCubeTexture(this->textureFileNames, "SkyMap");

	DEBUG << "Loading the small cube mesh object" << std::endl;
	this->loadMesh("smallCube.obj");

	return true;
}

void SkyMapManager::bindSkyMap(std::string samplerName) {
	//INFO << "Bind the skyMap to texture name: " << samplerName << std::endl;
	//this->getShaderManager().getActiveShaderManager()->bindTexture(samplerName, this->textureBufferID, GL_TEXTURE_CUBE_MAP);
	this->setCubeTexture(samplerName, "SkyMap");
}

void SkyMapManager::renderSkyMap(void) {
	this->renderMesh("smallCube.obj");
}

SkyMapManager::~SkyMapManager(void) {
}
