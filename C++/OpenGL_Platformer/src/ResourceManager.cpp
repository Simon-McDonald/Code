/*
 * ObjLoader.cpp
 *
 *  Created on: Mar 25, 2016
 *      Author: Simon
 */

#include "ResourceManager.hpp"

#include <string>

#include <CheckErrors.h>

ResourceManager::ResourceManager(void) {
    glGenVertexArrays (1, &this->vertexArrayID);
    glBindVertexArray (this->vertexArrayID);
}

SDL_Surface* ResourceManager::loadRawImage(std::string imageFileName) {
	std::string texturePath = ResourceManager::getConfig().getValue<std::string>("RESOURCES", "images_dir") + "\\" + imageFileName;

	SDL_Surface *textureData = IMG_Load(texturePath.c_str());
	if (!textureData) {
		ERR << "Failed to load image: '" << texturePath << "'" << std::endl;
		return nullptr;
	} else {
		INFO << "Loaded image '" << texturePath << "'" << std::endl;
	}

	return textureData;
}

GLuint ResourceManager::dataToTexture(std::string textureFileName, SDL_Surface *textureData) {
	GLuint textureBufferID;
	glGenTextures(1, &textureBufferID);

	glBindTexture(GL_TEXTURE_2D, textureBufferID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->w, textureData->h, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData->pixels);

	SDL_FreeSurface (textureData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);

	return textureBufferID;
}

GLuint ResourceManager::loadTexture(const std::string &textureFileName) {
	std::string texturePath = ResourceManager::getConfig().getValue<std::string>("RESOURCES", "images_dir") + "\\" + textureFileName;

	SDL_Surface *textureData = IMG_Load(texturePath.c_str());
	if (!textureData) {
		ERR << "Failed to load image: '" << texturePath << "', error: '" << IMG_GetError() << "'" << std::endl;
		return -1u;
	} else {
		INFO << "Loaded image '" << texturePath << "'" << std::endl;
	}

	GLuint textureBufferID;
	glGenTextures(1, &textureBufferID);

	glBindTexture(GL_TEXTURE_2D, textureBufferID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->w, textureData->h, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData->pixels);

	SDL_FreeSurface (textureData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureBufferID;
}

void ResourceManager::deleteTexture(const GLuint textureId) {
    glDeleteTextures(1, &textureId);
}

ResourceManager::~ResourceManager(void) {
    glDeleteVertexArrays(1, &this->vertexArrayID);
    this->vertexArrayID = -1;
}
