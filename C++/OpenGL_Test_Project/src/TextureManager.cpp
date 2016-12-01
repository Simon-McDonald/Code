/*
 * TextureManager.cpp
 *
 *  Created on: Jun 18, 2016
 *      Author: Simon
 */

#include "TextureManager.h"

#include <iostream>

TextureManager::TextureManager(std::string shaderLocName, GLenum textureUnit) :
	uniformName(shaderLocName), managedTextureUnit(textureUnit), uniformID(-1u) {
} /* TextureManager::TextureManager */

void TextureManager::Initialise(void) {
	//if (!ShaderManager::getActiveShaderManager()) {
	//	std::cout << "The current shader is null" << std::endl;
	//} else {
		std::cout << "The current shader is good" << std::endl;

		this->uniformID = ShaderManager::getActiveShaderManager()->getUniformLocation(this->uniformName);

		if (this->uniformID == 0xFFFFFFFF) {
			std::cout << "Error getting texture location, do nothing with texture" << std::endl;
		} else {
			std::cout << "The texture unit used: " << (this->managedTextureUnit - GL_TEXTURE0) << std::endl;
			glUniform1i(this->uniformID, this->managedTextureUnit - GL_TEXTURE0);
		}
	//}
} /* TextureManager::Initialise */

void TextureManager::BindTexture(GLuint textureBufferID) {
	if (this->uniformID != 0xFFFFFFFF) {
		glActiveTexture(this->managedTextureUnit);
		glBindTexture(GL_TEXTURE_2D, textureBufferID);
	}
} /* TextureManager::BindTexture */

TextureManager::~TextureManager(void) {}
