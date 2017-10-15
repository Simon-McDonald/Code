/*
 * Camera.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: Simon
 */

#include "Camera.h"
#include <stdio.h>

void Camera::Initialise(GeometryShader *geometryShader) {
	this->uniView = glGetUniformLocation(geometryShader->getShaderProgram(), "view");
    this->uniProj = glGetUniformLocation(geometryShader->getShaderProgram(), "proj");

    this->view = glm::lookAt(
    	glm::vec3(0.0f, 0.00001f, -2.0f), //eye
        glm::vec3(0.0f, 0.0f, 0.0f), //centre
        glm::vec3(0.0f, 0.0f, -1.0f) //up
    );

    /*std::cout << "[" << this->view[0][0] << ", " << this->view[0][1] << ", " << this->view[0][1] << ", " << this->view[0][3] << std::endl;
    std::cout << "[" << this->view[1][0] << ", " << this->view[1][1] << ", " << this->view[1][1] << ", " << this->view[1][3] << std::endl;
    std::cout << "[" << this->view[2][0] << ", " << this->view[2][1] << ", " << this->view[2][1] << ", " << this->view[2][3] << std::endl;
    std::cout << "[" << this->view[3][0] << ", " << this->view[3][1] << ", " << this->view[3][1] << ", " << this->view[3][3] << std::endl;*/

	//this->view = glm::mat4(1.0);
	//this->view[2][3] = -1 * this->zOff;
    glUniformMatrix4fv(this->uniView, 1, GL_FALSE, glm::value_ptr(view));

    /*view = glm::lookAt(
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(0.0f, 0.0f, 1.0f)
    );*/

    glm::mat4 proj = glm::perspective(glm::radians(30.0f), 800.0f / 600.0f, 0.1f, 10.0f);

    //this->proj[0][0] = this->n/this->r;

    //glm::mat4 proj = glm::mat4(1.0);
    glUniformMatrix4fv(this->uniProj, 1, GL_FALSE, glm::value_ptr(proj));


    /*proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
    std::cout << "---" << view << std::endl;*/

    this->vertRotate = glm::vec3(1.0f, 0.0f, 0.0f);
    this->horzRotate = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::vec3 Camera::getVertRotate() {
	return this->vertRotate;
}

glm::vec3 Camera::getHorzRotate() {
	return this->horzRotate;
}

void Camera::Update(float timeChange, UserInputStruct *userInputStruct) {
	float horzRotateAngle = 0;
	float vertRotateAngle = 0;

	if (userInputStruct->left) {
		horzRotateAngle += -1.0f * rotationRate;
	}
	if (userInputStruct->right) {
		horzRotateAngle += rotationRate;
	}

	if (userInputStruct->up) {
		vertRotateAngle += rotationRate;
	}
	if (userInputStruct->down) {
		vertRotateAngle += -1.0 * rotationRate;
	}

	if (userInputStruct->rightMouse) {
		horzRotateAngle += ((float) (userInputStruct->prevMousePosX - userInputStruct->mousePosX))/ 2.0;
		vertRotateAngle += -1 * ((float) (userInputStruct->prevMousePosY - userInputStruct->mousePosY))/ 2.0;
	}

	this->view = glm::rotate(this->view, glm::radians(horzRotateAngle), this->horzRotate);
	this->view = glm::rotate(this->view, glm::radians(vertRotateAngle), this->vertRotate);
	this->horzRotate = glm::rotate(this->horzRotate, glm::radians(-1 * vertRotateAngle), this->vertRotate);
	this->vertRotate = glm::rotate(this->vertRotate, glm::radians(-1 * horzRotateAngle), this->horzRotate);

	glUniformMatrix4fv(this->uniView, 1, GL_FALSE, glm::value_ptr(view));
}
