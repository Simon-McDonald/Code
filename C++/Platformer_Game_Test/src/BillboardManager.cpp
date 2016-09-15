/*
 * BillboardManager.cpp
 *
 *  Created on: Jul 19, 2016
 *      Author: Simon
 */

#include "BillboardManager.h"

BillboardManager::BillboardManager(void) : BillboardManager("") {
} /* BillboardManager::BillboardManager */

BillboardManager::BillboardManager(std::string imageFile) : pointBufferID(-1u), numVertices(0), imageName(imageFile) {
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
	concatPoints.reserve(numPoints);

	for (std::vector<std::pair<std::string, std::vector<GLfloat>>>::iterator vecItr = resourcesVector.begin();
			vecItr != resourcesVector.end();
			vecItr++) {
		concatPoints.insert(concatPoints.end(), vecItr->second.begin(), vecItr->second.end());
	}

	glGenBuffers(1, &pointBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, pointBufferID);
	glBufferData(GL_ARRAY_BUFFER, concatPoints.size() * sizeof(glm::vec3), concatPoints.data(), GL_STATIC_DRAW);

	this->numVertices = resourcesVector.size();


	/*std::vector<std::string> fileNames;
	fileNames.reserve(resourcesVector.size());

	for (std::vector<std::pair<std::string, std::vector<GLfloat>>>::iterator vecItr = resourcesVector.begin();
			vecItr != resourcesVector.end();
			vecItr++) {
		fileNames.push_back(vecItr->first);
	}

	this->generateTexture("TestTexture1", fileNames);*/

	return true;
}

void BillboardManager::Render(void) {
	if (this->imageName != "") {
		this->setTexture(this->imageName);
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->pointBufferID);
	glVertexAttribPointer(
		0,                  // attribute
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	ShaderManager::getActiveShaderManager()->validateProgram();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, this->numVertices/2);
}
