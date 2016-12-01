/*
 * FramebufferManager.h
 *
 *  Created on: Jun 21, 2016
 *      Author: Simon
 */

#ifndef FRAMEBUFFERMANAGER_H_
#define FRAMEBUFFERMANAGER_H_

#include "UtilityManager.h"
#include "WorldManager.h"

class FramebufferManager : protected UtilityManager, protected WorldManager {
public:
	enum managerType {
		colour,
		depth
	};

	FramebufferManager(void);

	void Initialise(void);

	void WriteTexture(void);
	void defaultFramebuffer(void);

	void ReadTexture(GLuint samplerID);
	void ReadTexture(std::string samplerName);

	GLuint getTextureID(void);

protected:
	GLuint textureID;
	GLuint renderbufferID;
	GLuint fboID;
};

#endif /* FRAMEBUFFERMANAGER_H_ */
