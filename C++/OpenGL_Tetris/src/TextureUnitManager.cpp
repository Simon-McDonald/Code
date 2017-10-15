/*
 * TextureManager.cpp
 *
 *  Created on: Jun 18, 2016
 *      Author: Simon
 */

#include "TextureUnitManager.hpp"

TextureUnitManager::TextureUnitManager(std::string textureUnitName) : textureUnitName(textureUnitName) {}

bool TextureUnitManager::bindTexture(TextureManager &textureTarget) {
	return this->getShaderManager().bindTexture(this->textureUnitName, textureTarget.getBufferID(), textureTarget.getTextureType());
}

bool TextureUnitManager::bindTexture(GLuint textureBufferID, GLenum textureType) {
	return this->getShaderManager().bindTexture(this->textureUnitName, textureBufferID, textureType);
}

TextureUnitManager::~TextureUnitManager(void) {}
