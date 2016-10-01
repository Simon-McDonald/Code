/*
 * Camera.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: Simon
 */

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera() :
	cameraPosition(glm::vec2(0, 0)) {

	screenHeightPixels = this->getConfig().getInt("WINDOW", "window_height");
	screenWidthPixels = this->getConfig().getInt("WINDOW", "window_width");
	blockSizePixels = this->getConfig().getInt("WINDOW", "block_size");
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
	glm::mat4 tempMat = glm::translate(glm::mat4(1.0), glm::vec3(-1 * this->cameraPosition.x, -1 * this->cameraPosition.y, 0.0));
	return tempMat;
}

void Camera::SetViewMatrix(void) {
	glm::mat4 view = this->GetViewProjMatrix();
	this->getShaderManager().bindArray("cameraView", 1, GL_FALSE, &view[0][0]);

	this->getShaderManager().setUniformFloat("screenWidth", this->screenWidthPixels / this->blockSizePixels);
	this->getShaderManager().setUniformFloat("screenHeight", this->screenHeightPixels / this->blockSizePixels);
}

Camera::~Camera(void) {
}
