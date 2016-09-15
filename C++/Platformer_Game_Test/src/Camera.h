/*
 * Camera.h
 *
 *  Created on: Oct 18, 2015
 *      Author: Simon
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>

#include "UtilityManager.h"
#include "WorldManager.h"

class Camera : UtilityManager, WorldManager {

public:
	Camera(void);

	void Reset(void);
	void Update(glm::vec2);
	void Set(glm::vec2);

	glm::vec2 GetPosition(void);
	glm::mat4 GetViewProjMatrix(void);

	~Camera(void);

private:
	glm::vec2 cameraPosition;
};

#endif /* CAMERA_H_ */
