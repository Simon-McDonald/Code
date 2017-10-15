/*
 * BillboardManager.cpp
 *
 *  Created on: Jul 19, 2016
 *      Author: Simon
 */

#include "BillboardManager.h"

BillboardManager::BillboardManager(std::string imageFile) : pointBufferID(-1u), numVertices(0), imageName(imageFile) {
} /* BillboardManager::BillboardManager */

BillboardManager::~BillboardManager(void) {
} /* BillboardManager::~BillboardManager */

bool BillboardManager::InitialiseBillboard(std::vector<GLfloat> locationsVector) {
	this->loadTexture(this->imageName);

	glGenBuffers(1, &pointBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, pointBufferID);
	glBufferData(GL_ARRAY_BUFFER, locationsVector.size() * sizeof(glm::vec3), &locationsVector[0], GL_STATIC_DRAW);

	this->numVertices = locationsVector.size();

	return true;
}

void BillboardManager::Render(void) {
	if (this->imageName != "") {
		this->setTexture(this->imageName);
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->pointBufferID);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	ShaderManager::getActiveShaderManager()->validateProgram();

	glDrawArrays(GL_POINTS, 0, this->numVertices/3);
}
