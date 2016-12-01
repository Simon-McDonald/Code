/*
 * RenderableObject.h
 *
 *  Created on: Dec 29, 2015
 *      Author: Simon
 */

#ifndef RENDERABLEOBJECT_H_
#define RENDERABLEOBJECT_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObject.h"
#include "Renderer.h"

class Renderer;

class RenderableObject : public GameObject {

public:
	RenderableObject(std::string meshFileName, std::string textureFileName, glm::vec3 initPosition, float scale);
	RenderableObject(glm::vec3 initPosition, float scale);

	void Render(void);

	glm::mat4 getViewMatrix(void);
	glm::vec3 getWorldLoc(void);

protected:
	glm::vec3 worldLocation;
	float modelScale;
	float horizontalAngle;
	float verticalAngle;

	std::string meshFile;
	std::string textureFile;
};

#endif /* RENDERABLEOBJECT_H_ */
