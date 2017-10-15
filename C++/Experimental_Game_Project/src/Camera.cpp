/*
 * Camera.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: Simon
 */

#include "Camera.h"
#include <stdio.h>

const float Camera::translationRate = 8.0f;
const float Camera::rotationRate = 0.1f;

Camera::Camera() : fieldView(45), horizontalAngle(3.14f), verticalAngle(0), cameraPosition(glm::vec3(0, 0, 5)) {
}

void Camera::Update(float deltaTime) {
	UserInputStruct &input = this->getWindow().getInput();
	int xpos = input.mousePosX, ypos = input.mousePosY;

	this->getWindow().setMousePos (this->getWindow().getWidth()/2,
								   this->getWindow().getHeight()/2);

	/*this->horizontalAngle += Camera::rotationRate * deltaTime *
		float(this->getWindow().getWidth()/2 - xpos);
	this->verticalAngle += Camera::rotationRate * deltaTime *
		float(this->getWindow().getHeight()/2 - ypos);*/

	glm::vec3 direction(
	    cos(verticalAngle) * sin(horizontalAngle),
	    0,
	    cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
	    sin(horizontalAngle - 3.14f/2.0f),
	    0,
	    cos(horizontalAngle - 3.14f/2.0f)
	);

	// Move forward
	if (input.onClick.up){
		this->cameraPosition += direction * deltaTime * Camera::translationRate;
	}
	// Move backward
	if (input.onClick.down){
		this->cameraPosition -= direction * deltaTime * Camera::translationRate;
	}
	// Strafe right
	if (input.onClick.right){
		this->cameraPosition += right * deltaTime * Camera::translationRate;
	}
	// Strafe left
	if (input.onClick.left){
	    this->cameraPosition -= right * deltaTime * Camera::translationRate;
	}
}

glm::mat4 Camera::getProjectionMatrix(void) {
	return glm::perspective(this->fieldView, 4.0f / 3.0f, 0.1f, 150.0f);
}

glm::mat4 Camera::getViewMatrix(void) {
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	glm::vec3 right = glm::vec3(
	    sin(horizontalAngle - 3.14f/2.0f),
	    0,
	    cos(horizontalAngle - 3.14f/2.0f)
	);

	glm::vec3 up = glm::cross( right, direction );

	return glm::lookAt(
		this->cameraPosition,
		this->cameraPosition+direction,
		up
	);
}

glm::mat4 Camera::getMatrixProduct(void) {
	return this->getViewMatrix() * this->getProjectionMatrix();
}
