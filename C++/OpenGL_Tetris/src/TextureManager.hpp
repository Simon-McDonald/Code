/*
 * TextureManager.h
 *
 *  Created on: Jun 18, 2016
 *      Author: Simon
 */

#ifndef TEXTUREMANAGER_HPP_
#define TEXTUREMANAGER_HPP_

#include <string>
#include <array>

#include <GL/glew.h>

#include <ShaderManager.hpp>

#include <UtilityManager.hpp>

class TextureManager : protected UtilityManager {
public:
	TextureManager(void);

	void initTextureManager(std::string textureFileFullPath);
	void initTextureManager(std::array<std::string, 6> &textureFiles, std::string &folderPath);
	void initTextureManager(unsigned length, void* data, GLenum dataType);

	void initTextureManager(GLuint existingTextureBufferID, GLenum textureType);

	void resetTextureManager(void);

	GLuint getBufferID(void);
	GLenum getTextureType(void);

	~TextureManager(void);

protected:
	GLuint textureBufferID;
	GLenum textureType;
	bool bufferOwner;

	/*std::string uniformName;
	GLenum managedTextureUnit;
	GLuint uniformID;*/
};

#endif /* TEXTUREMANAGER_HPP_ */
