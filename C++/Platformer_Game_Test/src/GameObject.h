/*
 * GameObject.h
 *
 *  Created on: May 25, 2016
 *      Author: Simon
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <map>

#include "ResourceManager.h"

class GameObject : protected ResourceManager {
public:
	GameObject();

	int getMyId();

	virtual void Update(float deltaTime);

	virtual ~GameObject();

private:
	static int nextId;
	int id;

};

#endif /* GAMEOBJECT_H_ */
