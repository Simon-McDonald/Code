/*
 * Camera.h
 *
 *  Created on: Oct 18, 2015
 *      Author: Simon
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ShaderManager.h"
#include "OpenGLWindow.h"

#include "UtilityManager.h"
#include "WorldManager.h"

class Camera : UtilityManager, WorldManager {

public:
	Camera(void);
	void Update(float deltaTime);

	glm::mat4 getProjectionMatrix(void);
	glm::mat4 getViewMatrix(void);
	glm::mat4 getMatrixProduct(void);

private:
	static const float translationRate;
	static const float rotationRate;

	float fieldView;
	float horizontalAngle;
	float verticalAngle;

	glm::vec3 cameraPosition;
};

#endif /* CAMERA_H_ */
