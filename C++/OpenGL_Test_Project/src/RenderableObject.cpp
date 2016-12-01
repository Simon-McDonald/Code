/*
 * RenderableObject.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: Simon
 */

#include "RenderableObject.h"

RenderableObject::RenderableObject(std::string meshFileName, std::string textureFileName, glm::vec3 initPosition, float scale) :
	worldLocation(initPosition), modelScale(scale), horizontalAngle(0), verticalAngle(0), meshFile(meshFileName), textureFile(textureFileName) {

	if (textureFileName != "")
		this->loadTexture(textureFileName);
	if (meshFileName != "")
		this->loadMesh(meshFileName);
}

RenderableObject::RenderableObject(glm::vec3 initPosition, float scale) :
	worldLocation(initPosition), modelScale(scale), horizontalAngle(0), verticalAngle(0), meshFile("") {
}

void RenderableObject::Render(void) {
	if (this->textureFile != "") {
		this->setTexture(this->textureFile);
	}

	if (this->meshFile != "") {
		this->renderMesh(this->meshFile);
	}
}

glm::vec3 RenderableObject::getWorldLoc(void) {
	return this->worldLocation;
}

glm::mat4 RenderableObject::getViewMatrix(void) {
	glm::mat4 modelMatrix = glm::diagonal4x4(glm::vec4(glm::vec3(this->modelScale), 1.0f));
	modelMatrix = glm::rotate(modelMatrix, this->verticalAngle, glm::vec3(1, 0, 0));
	modelMatrix = glm::rotate(modelMatrix, this->horizontalAngle, glm::vec3(0, 1, 0));
	modelMatrix[3] = glm::vec4(this->worldLocation, 1);

	return modelMatrix;
}
