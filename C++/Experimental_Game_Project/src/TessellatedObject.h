/*
 * tessellatedObject.h
 *
 *  Created on: Nov 24, 2016
 *      Author: Simon
 */

#ifndef TESSELLATEDOBJECT_H_
#define TESSELLATEDOBJECT_H_

#include "ResourceManager.h"

class TessellatedObject : protected ResourceManager {
public:
	TessellatedObject(std::string meshFile, std::string textureFile, glm::vec3 worldLocation);

	glm::mat4 getViewMatrix(void);

	void render(glm::mat4 viewProjMat);

	~TessellatedObject(void);

private:
	glm::vec3 worldLocation;
	std::string textureFile;
	std::string meshFile;
};

#endif /* TESSELLATEDOBJECT_H_ */
