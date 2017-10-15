/*
 * Camera.h
 *
 *  Created on: Oct 18, 2015
 *      Author: Simon
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "OpenGLController.h"
#include "GeometryShader.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {

public:
	void Initialise(GeometryShader*);
	void Update(float, UserInputStruct*);

	float rotationRate = 0.5f;
	float zOff = 1.0f;

	glm::vec3 getVertRotate();
	glm::vec3 getHorzRotate();

private:
	GLint uniView;
    GLint uniProj;

    glm::mat4 view;
    glm::mat4 proj;

    glm::vec3 vertRotate;
    glm::vec3 horzRotate;

};

#endif /* CAMERA_H_ */
