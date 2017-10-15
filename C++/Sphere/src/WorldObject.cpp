/*
 * WorldObject.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: Simon
 */

#include <iostream>
#include "WorldObject.h"

int generateId() {
	static int intId = 1000;
	intId++;
	return intId;
}

WorldObject::WorldObject() : entityId(ID), entityPos() {}

WorldObject::WorldObject(WorldPosition position) : entityId(ID), entityPos(position) {}

WorldPosition* WorldObject::getPosition() {
	return &this->entityPos;
}

void WorldObject::moveObject(float timeChange, UserInputStruct *userInputStruct, glm::vec3 horzRotate, glm::vec3 vertRotate) {
	//std::cout << "Initial: Latitude: " << this->entityPos.getLatitude() << ", Longitude: " << this->entityPos.getLongitude() << ", Radius: " << this->entityPos.getRadius() << std::endl;

	float horzRotateAngle = 0;
	float vertRotateAngle = 0;

	if (userInputStruct->left) {
		horzRotateAngle += -1.0f * this->rotationRate;
	}
	if (userInputStruct->right) {
		horzRotateAngle += this->rotationRate;
	}

	if (userInputStruct->up) {
		vertRotateAngle += this->rotationRate;
	}
	if (userInputStruct->down) {
		vertRotateAngle += -1.0 * this->rotationRate;
	}

	glm::mat4 rotationMatrix(1);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(horzRotateAngle), horzRotate);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(vertRotateAngle), vertRotate);

	this->entityPos.setPosition(glm::vec4(rotationMatrix * this->entityPos.getCartesianCoords()));

	//std::cout << "Final: Latitude: " << this->entityPos.getLatitude() << ", Longitude: " << this->entityPos.getLongitude() << ", Radius: " << this->entityPos.getRadius() << std::endl;
}

int WorldObject::getId() {
	return this->entityId;
}
