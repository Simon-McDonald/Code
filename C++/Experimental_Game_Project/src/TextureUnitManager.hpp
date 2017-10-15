/*
 * TextureManager.h
 *
 *  Created on: Jun 18, 2016
 *      Author: Simon
 */

#ifndef TEXTUREUNITMANAGER_H_
#define TEXTUREUNITMANAGER_H_

#include <string>

#include <GL/glew.h>

#include <UtilityManager.hpp>
#include <WorldManager.hpp>

#include "TextureManager.hpp"

class TextureUnitManager : protected UtilityManager, protected WorldManager {
public:
	TextureUnitManager(std::string textureUnitName);

	bool bindTexture(TextureManager &textureTarget);
	bool bindTexture(GLuint textureBufferID, GLenum textureType);

	~TextureUnitManager(void);

protected:
	std::string textureUnitName;
};

#endif /* TEXTUREUNITMANAGER_H_ */
