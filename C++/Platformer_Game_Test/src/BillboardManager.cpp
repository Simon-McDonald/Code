/*
 * BillboardManager.cpp
 *
 *  Created on: Jul 19, 2016
 *      Author: Simon
 */

#include <chrono>
#include <iterator>

#include "BillboardManager.h"

#include "CheckErrors.h"

BillboardManager::BillboardManager(void) : pointBufferID(-1u), numVertices(0), imageID(-1u) {
} /* BillboardManager::BillboardManager */

BillboardManager::~BillboardManager(void) {
} /* BillboardManager::~BillboardManager */

bool BillboardManager::InitialiseBillboard(
		std::vector<std::pair<std::string, std::vector<GLfloat>>> &resourcesVector) {

	int numPoints = 0;

	for (std::vector<std::pair<std::string, std::vector<GLfloat>>>::iterator vecItr = resourcesVector.begin();
			vecItr != resourcesVector.end();
			vecItr++) {
		numPoints += vecItr->second.size();
	}

	std::vector<GLfloat> concatPoints;
	concatPoints.reserve(numPoints * 3 / 2);

	for (std::vector<std::pair<std::string, std::vector<GLfloat>>>::iterator vecItr = resourcesVector.begin();
			vecItr != resourcesVector.end();
			vecItr++) {

		for (std::vector<GLfloat>::iterator flVecItr = vecItr->second.begin();
				flVecItr != vecItr->second.end();
				flVecItr = std::next(flVecItr, 2))
		{
			concatPoints.push_back(*flVecItr);
			concatPoints.push_back(*std::next(flVecItr));
			concatPoints.push_back(vecItr - resourcesVector.begin());
		}
	}

	glGenBuffers(1, &pointBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, pointBufferID);
	glBufferData(GL_ARRAY_BUFFER, concatPoints.size() * sizeof(glm::vec3), concatPoints.data(), GL_STATIC_DRAW);

	this->numVertices = concatPoints.size() / 3;

	std::vector<std::string> fileNames;
	fileNames.reserve(resourcesVector.size());

	for (std::vector<std::pair<std::string, std::vector<GLfloat>>>::iterator vecItr = resourcesVector.begin();
			vecItr != resourcesVector.end();
			vecItr++) {
		fileNames.push_back(vecItr->first);
	}

	this->imageID = this->generateTexture(fileNames);
	this->textureGridDim = this->determineTextureGridSize(fileNames.size());

	return true;
}

void BillboardManager::Render(void) {
	if (this->imageID != -1u) {
		this->setTexture(this->imageID);
	}

	CHECKERRORS();

	glBindBuffer(GL_ARRAY_BUFFER, this->pointBufferID);
	glVertexAttribPointer(
		0,                  // attribute
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof (GLfloat), // stride
		(void*)0            // array buffer offset
	);

	glVertexAttribPointer(
		1,                  // attribute
		1,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof (GLfloat), // stride
		(void*)(2 * sizeof (GLfloat)) // array buffer offset
	);

	CHECKERRORS();

	this->getShaderManager().setUniformFloat("textureGridDim", textureGridDim);
	this->getShaderManager().setUniformBool("flipVertical", false);

	ShaderManager::getActiveShaderManager()->validateProgram();

	glDrawArrays(GL_POINTS, 0, this->numVertices);
}
