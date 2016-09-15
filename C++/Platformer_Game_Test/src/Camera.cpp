/*
 * Camera.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: Simon
 */

#include "Camera.h"

Camera::Camera() :
	cameraPosition(glm::vec2(0, 0)) {
}

void Camera::Reset(void) {
	this->cameraPosition = glm::vec2(0, 0);
}

void Camera::Update(glm::vec2 deltaPosition) {
	this->cameraPosition += deltaPosition;
}

void Camera::Set(glm::vec2 deltaPosition) {
	this->cameraPosition = deltaPosition;
}

glm::vec2 Camera::GetPosition(void) {
	return this->cameraPosition;
}

glm::mat4 Camera::GetViewProjMatrix(void) {
	glm::mat4 tempMat = glm::mat4(1.0);
	return tempMat;
}

Camera::~Camera(void) {
}
