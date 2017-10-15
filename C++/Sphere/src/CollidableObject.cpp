/*
 * CollidableObject.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: Simon
 */

#include "CollidableObject.h"

CollidableObject::CollidableObject(int numLayers, WorldPosition position, float scale, Renderer *renderer, int colour) : RenderableObject(numLayers, position, scale, renderer, colour) {
}

void CollidableObject::killOnContact(CollidableObject *object) {
	if (!this->checkIfAlive()) {
		return;
	}

	glm::vec4 pos1 = object->getPosition()->getCartesianCoords();
	float rad1 = object->getObjectRadius();

	glm::vec4 pos2 = this->getPosition()->getCartesianCoords();
	float rad2 = this->getObjectRadius();

	//std::cout << "Player: x: " << pos1[0] << ", y: " << pos1[1] << ", z: " << pos1[2] << ", r: " << rad1 << std::endl;
	//std::cout << "Enemy : x: " << pos2[0] << ", y: " << pos2[1] << ", z: " << pos2[2] << ", r: " << rad2 << std::endl;

	float dist = 0;
	for (int index = 0; index < 3; index++) {
		dist += (pos1[index] - pos2[index]) * (pos1[index] - pos2[index]);
	}

	float limit = (rad1 + rad2) * (rad1 + rad2);

	if (dist < limit) {
		this->kill();
	}

	//std::cout << "Distance sq: " << dist << ", limit: " << limit << std::endl;
}
