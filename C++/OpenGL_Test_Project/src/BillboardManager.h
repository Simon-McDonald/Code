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
	BillboardManager(std::string imageFile);
	~BillboardManager(void);

	bool InitialiseBillboard(std::vector<GLfloat> locationsVector);

	void Render(void);

private:
	GLuint pointBufferID;
	size_t numVertices;
	std::string imageName;
};

#endif /* BILLBOARDMANAGER_H_ */
