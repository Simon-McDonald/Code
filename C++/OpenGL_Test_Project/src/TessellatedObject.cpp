/*
 * tessellatedObject.cpp
 *
 *  Created on: Nov 24, 2016
 *      Author: Simon
 */

#include "TessellatedObject.h"

#include "CheckErrors.h"

TessellatedObject::TessellatedObject(std::string meshFile, std::string textureFile, glm::vec3 worldLocation) :
	worldLocation(worldLocation), textureFile(textureFile), meshFile(meshFile) {

	if (this->textureFile != "") {
		this->loadTexture(this->textureFile);
	}
	if (this->meshFile != "") {
		this->loadMesh(this->meshFile);
	}
}

glm::mat4 TessellatedObject::getViewMatrix(void) {
	glm::mat4 modelMatrix = glm::mat4();
	modelMatrix[3] = glm::vec4(this->worldLocation, 1);

	return modelMatrix;
}

void TessellatedObject::render(glm::mat4 viewProjMat) {

	//glPolygonMode(GL_BACK, GL_LINE);


	static float tessLevel = 1.0;
	static int tick = 0;
	if (this->getWindow().getInput().space && !tick) {
		tessLevel += 1.0;
		tick = 50;
		INFO << "New Tess Level: " << tessLevel;
	}
	if (tick != 0)
	{
		tick--;
	}

	/*if (this->getWindow().getInput().left) {
		tessLevel -= 1.0;
		if (tessLevel < 1.0) {
			tessLevel = 1.0;
		}
		INFO << "New Tess Level: " << tessLevel << std::endl;
	}*/

	CHECKERRORS();
	glm::mat4 modelMatrix = this->getViewMatrix();
	this->getShaderManager().bindArray("gWorld", 1, GL_FALSE, &modelMatrix[0][0]);
	CHECKERRORS();
	this->getShaderManager().setUniformFloat("gTessellationLevel", tessLevel);
	CHECKERRORS();
	this->getShaderManager().bindArray("gVP", 1, GL_FALSE, &viewProjMat[0][0]);

	if (this->textureFile != "") {
		this->setTexture(this->textureFile);
	}

	CHECKERRORS();

	if (this->meshFile != "") {
		this->renderMesh(this->meshFile, true);
	}


	//glPolygonMode(GL_BACK, GL_FILL);
	CHECKERRORS();
}

TessellatedObject::~TessellatedObject(void) {

}
