/*
 * TextureManager.cpp
 *
 *  Created on: Jun 18, 2016
 *      Author: Simon
 */

#include <iostream>

#include <SDL_image.h>

#include "TextureManager.hpp"

TextureManager::TextureManager(void) :
		textureBufferID(-1u), textureType(GL_TEXTURE_2D), bufferOwner(false) {}

void TextureManager::initTextureManager(std::string textureFileFullPath) {
	this->textureBufferID = -1u;
	this->textureType = GL_TEXTURE_2D;
	this->bufferOwner = true;

	SDL_Surface *textureData = IMG_Load(textureFileFullPath.c_str());
	if (!textureData) {
		ERR << "Failed to load image: '" << textureFileFullPath << "'" << END;
	} else {
		INFO << "Loaded image '" << textureFileFullPath << "'" << END;
		glGenTextures(1, &this->textureBufferID);

		glBindTexture(GL_TEXTURE_2D, this->textureBufferID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->w, textureData->h, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData->pixels);

		SDL_FreeSurface (textureData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		// For pixelated images
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void TextureManager::initTextureManager(std::array<std::string, 6> &textureFiles, std::string &folderPath) {
	this->textureBufferID = -1u;
	this->textureType = GL_TEXTURE_CUBE_MAP;
	this->bufferOwner = true;

	std::string imageDirPath = folderPath + "\\";

	static const GLenum types[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
									  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
									  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
									  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
									  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
									  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

	glGenTextures(1, &this->textureBufferID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureBufferID);

	for (unsigned int arrIdx = 0; arrIdx < textureFiles.size(); arrIdx++) {
		std::string imageLoc = imageDirPath + textureFiles.at(arrIdx);
		SDL_Surface *textureData = IMG_Load(imageLoc.c_str());
		if (!textureData) {
			ERR << "Failed to load image: '" << imageLoc << "'" << END;
			return;
		} else {
			DEBUG << "Loaded image: '" << imageLoc << "', used for cube texture: '" << END;
		}

		glTexImage2D(types[arrIdx], 0, GL_RGB, textureData->w, textureData->h, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData->pixels);

		SDL_FreeSurface (textureData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);
	//glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void TextureManager::initTextureManager(unsigned length, void* data, GLenum dataType) {
	this->textureBufferID = -1u;
	this->textureType=GL_TEXTURE_1D;
	this->bufferOwner=true;

	glGenTextures(1, &this->textureBufferID);
	glBindTexture(GL_TEXTURE_1D, this->textureBufferID);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, length, 0.0f, GL_RGB, GL_FLOAT, data);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);
}

void TextureManager::initTextureManager(GLuint existingTextureBufferID, GLenum textureType) {
	this->textureBufferID = existingTextureBufferID;
	this->textureType = textureType;
	this->bufferOwner = false;
}

void TextureManager::resetTextureManager(void) {
	if (this->bufferOwner) {
		glDeleteTextures(1, &this->textureBufferID);
	}

	this->textureBufferID = -1u;
	this->textureType = GL_TEXTURE_1D;
	this->bufferOwner = false;
}

GLuint TextureManager::getBufferID(void) {
	return this->textureBufferID;
}

GLenum TextureManager::getTextureType(void) {
	return GL_TEXTURE_2D;
}

TextureManager::~TextureManager(void) {
	if (this->bufferOwner) {
		glDeleteTextures(1, &this->textureBufferID);
	}
}
