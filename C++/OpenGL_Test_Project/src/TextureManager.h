/*
 * TextureManager.h
 *
 *  Created on: Jun 18, 2016
 *      Author: Simon
 */

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

#include <string>
#include "OpenGLWindow.h"
#include "ShaderManager.h"

#include "UtilityManager.h"
//#include "WorldManager.h"

class TextureManager : protected UtilityManager {
public:
	TextureManager(std::string shaderLocName, GLenum textureUnit);
	virtual void Initialise(void);

	void BindTexture(GLuint textureBufferID);

	virtual ~TextureManager(void);

protected:
	std::string uniformName;
	GLenum managedTextureUnit;
	GLuint uniformID;
};

#endif /* TEXTUREMANAGER_H_ */
