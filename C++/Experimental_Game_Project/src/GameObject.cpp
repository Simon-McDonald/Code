/*
 * GameObject.cpp
 *
 *  Created on: May 25, 2016
 *      Author: Simon
 */

#include "GameObject.h"

int GameObject::nextId = 0;

GameObject::GameObject() : id(nextId++) {
} /* GameObject::GameObject */

int GameObject::getMyId() {
	return this->id;
} /* GameObject::getMyId */

void GameObject::Update(float deltaTime) {
} /* GameObject::Update */

GameObject::~GameObject() {
} /* GameObject::~GameObject */
