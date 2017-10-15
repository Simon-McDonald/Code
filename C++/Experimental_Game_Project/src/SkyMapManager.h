/*
 * SkyMapManager.h
 *
 *  Created on: Jul 2, 2016
 *      Author: Simon
 */

#ifndef SKYMAPMANAGER_H_
#define SKYMAPMANAGER_H_

#include <array>

#include "ResourceManager.h"

class SkyMapManager : protected ResourceManager {
public:
	SkyMapManager(void);

	SkyMapManager(std::string xPlusTexture, std::string xNegTexture,
			      std::string yPlusTexture, std::string yNegTexture,
				  std::string zPlusTexture, std::string zNegTexture);

	bool initialiseSkyMap(void);

	bool initialiseSkyMap(std::string configItem);

	void bindSkyMap(std::string samplerName);

	void renderSkyMap(void);

	~SkyMapManager(void);

private:
	std::array<std::string, 6> textureFileNames;
	GLuint textureBufferID;
};

#endif /* SKYMAPMANAGER_H_ */
