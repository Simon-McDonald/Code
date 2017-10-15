/*
 * WorldObject.h
 *
 *  Created on: Dec 29, 2015
 *      Author: Simon
 */

#ifndef WORLDOBJECT_H_
#define WORLDOBJECT_H_

#include "WorldPosition.h"
#include "OpenGLController.h"

int generateId();
#define ID generateId()

class WorldObject {

public:
	float rotationRate = -0.5f;

	WorldObject();
	WorldObject(WorldPosition);

	WorldPosition* getPosition();

	int getId();

	void moveObject(float, UserInputStruct*, glm::vec3, glm::vec3);

private:
	int entityId;

protected:
	WorldPosition entityPos;

};

#endif /* WORLDOBJECT_H_ */
