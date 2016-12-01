/*
 * ShadowMapManager.h
 *
 *  Created on: Jun 19, 2016
 *      Author: Simon
 */

#ifndef SHADOWMAPMANAGER_H_
#define SHADOWMAPMANAGER_H_

#include "WorldManager.h"
#include "UtilityManager.h"

class ShadowMapManager : protected WorldManager, protected UtilityManager {
public:
	ShadowMapManager(void);

	bool InitialiseShadowMap(void);

	void BindTexture(void);

	void WriteTexture(void);

	void defaultFramebuffer(void);

	GLuint getTextureID(void);

	~ShadowMapManager(void);

protected:
	GLuint textureID;
	GLuint fboID;
};

#endif /* SHADOWMAPMANAGER_H_ */
