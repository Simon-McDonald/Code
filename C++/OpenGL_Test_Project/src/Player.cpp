/*
 * Player.cpp
 *
 *  Created on: Jun 12, 2016
 *      Author: Simon
 */

#include "Player.h"

const float Player::translationRate = 8.0f;
const float Player::rotationRate = 0.1f;

Player::Player(glm::vec3 initPosition, float scale) : RenderableObject(initPosition, scale), velocity(0.0, 0.0, 0.0), jumpDuration(0), jumpAnimation(false),
		fieldView(45), horizontalAngle(0.0f), verticalAngle(0) {
}

void Player::Update(float deltaTime) {
	UserInputStruct &input = this->getWindow().getInput();
	int xpos = input.mousePosX, ypos = input.mousePosY;

	this->getWindow().setMousePos (this->getWindow().getWidth()/2,
								   this->getWindow().getHeight()/2);

	this->horizontalAngle -= Player::rotationRate * deltaTime *
		float(this->getWindow().getWidth()/2 - xpos);
	this->verticalAngle += Player::rotationRate * deltaTime *
		float(this->getWindow().getHeight()/2 - ypos);

	//std::cout << "(" << this->horizontalAngle << "," << this->verticalAngle << std::endl;

	glm::vec3 direction(
	    cos(verticalAngle) * sin(horizontalAngle),
	    cos(verticalAngle) * cos(horizontalAngle),
		0
	);

	// Right vector
	glm::vec3 right = glm::vec3(
	    sin(horizontalAngle - 3.14f/2.0f),
	    cos(horizontalAngle - 3.14f/2.0f),
		0
	);

	// Move forward
	if (input.up){
		this->worldLocation += direction * deltaTime * Player::translationRate;
	}
	// Move backward
	if (input.down){
		this->worldLocation -= direction * deltaTime * Player::translationRate;
	}
	// Strafe right
	if (input.right){
		this->worldLocation -= right * deltaTime * Player::translationRate;
	}
	// Strafe left
	if (input.left){
	    this->worldLocation += right * deltaTime * Player::translationRate;
	}
}

glm::mat4 Player::getViewMatrix(void) {
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		cos(verticalAngle) * cos(horizontalAngle),
		sin(verticalAngle)

	);

	glm::vec3 right = glm::vec3(
	    sin(horizontalAngle - 3.14f/2.0f),
	    cos(horizontalAngle - 3.14f/2.0f),
		0
	);

	glm::vec3 up = glm::cross( direction, right );

	return glm::lookAt(
		this->worldLocation,
		this->worldLocation+direction,
		up
	);
}

glm::mat4 Player::getRotationMatrix(void) {
	glm::mat4 rotationMatrix = glm::mat4(1.0);
	rotationMatrix = glm::rotate(rotationMatrix, this->horizontalAngle, glm::vec3(0.0, 1.0, 0.0));
	rotationMatrix = glm::rotate(rotationMatrix, -1 * this->verticalAngle, glm::vec3(1.0, 0.0, 0.0));
	return rotationMatrix;
}

Player::~Player() {
}
