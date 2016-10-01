/*
 * BillboardManager.h
 *
 *  Created on: Jul 19, 2016
 *      Author: Simon
 */

#ifndef BILLBOARDMANAGER_H_
#define BILLBOARDMANAGER_H_

#include <vector>

#include "ResourceManager.h"

class BillboardManager : protected ResourceManager {
public:
	BillboardManager(void);
	BillboardManager(std::string imageFile);

	bool InitialiseBillboard(
		std::vector<std::pair<std::string, std::vector<GLfloat>>> &resourcesVector);

	void Render(void);

	~BillboardManager(void);

private:
	GLuint pointBufferID;
	size_t numVertices;
	GLuint imageID;
	float textureGridDim;
};

#endif /* BILLBOARDMANAGER_H_ */
