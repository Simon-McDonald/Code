/*
 * ObjLoader.cpp
 *
 *  Created on: Mar 25, 2016
 *      Author: Simon
 */

#include <algorithm>
#include <vector>
#include <string>
#include <cstring>

#include <SDL_image.h>

#include "CheckErrors.h"
#include "ShaderManager.h"

#include <iostream>
#include "ResourceManager.h"

std::map<std::string, GLuint> ResourceManager::textureResourceID;
std::map<GLuint, unsigned> ResourceManager::textureResourceStore;
GLuint ResourceManager::errorTexture = -1u;

ResourceManager::ResourceManager() {}

bool ResourceManager::InitialiseResourceManager(void) {
	ResourceManager::errorTexture = ResourceManager::generateErrorTexture();

	DEBUG << "Error texture ID: " << ResourceManager::errorTexture << std::endl;
	return true;
}

SDL_Surface* ResourceManager::getSDLSurface(std::string imageFileName) {
	std::string texturePath = this->getConfig().getString("RESOURCES", "images_dir") + "\\" + imageFileName;

	SDL_Surface *textureData = IMG_Load(texturePath.c_str());
	if (!textureData) {
		ERR << "Failed to load image: '" << texturePath << "', error: " << IMG_GetError() << std::endl;
	} else {
		INFO << "Loaded image '" << texturePath << "'" << std::endl;
	}

	return textureData;
}

GLuint ResourceManager::loadTexture(std::string textureFileName) {
	if (textureFileName.length() == 0) {
		return -1u;
	}

	if (ResourceManager::textureResourceID.count(textureFileName) != 0) {
		GLuint bufferID = ResourceManager::textureResourceID.at(textureFileName);

		assert (ResourceManager::textureResourceStore.count(bufferID) != 0);
		++ResourceManager::textureResourceStore.at(bufferID);

		return bufferID;
	}

	if (SDL_Surface* textureData = getSDLSurface(textureFileName)) {
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

		ResourceManager::textureResourceID.emplace(textureFileName, textureBufferID);
		ResourceManager::textureResourceStore.emplace(textureBufferID, 1);

		return textureBufferID;
	}

	return -1u;
} /* ResourceManager::loadTexture */

int ResourceManager::determineTextureGridSize(int numTextures) {
	return static_cast<int>(ceil(sqrt(static_cast<float>(numTextures))));
}

GLuint ResourceManager::generateErrorTexture(void) {
	int indTexDim = 16;
	int numPixels = indTexDim;

	std::vector<unsigned char> rawTexture;
	rawTexture.resize(3 * numPixels * numPixels);

	for (int texRowIdx = 0; texRowIdx < numPixels; texRowIdx++) {
		for (int texColIdx = 0; texColIdx < numPixels; texColIdx++) {
			int idx = texRowIdx * (numPixels * 3) + texColIdx * 3;

			rawTexture.at(idx) = 250;
			rawTexture.at(idx + 1) = 0;

			if ((texRowIdx + texColIdx) % 2) {
				rawTexture.at(idx + 2) = 255;
			} else {
				rawTexture.at(idx + 2) = 127;
			}
		}
	}

	GLuint textureBufferID;
	glGenTextures(1, &textureBufferID);
	glBindTexture(GL_TEXTURE_2D, textureBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, numPixels, numPixels, 0, GL_RGB, GL_UNSIGNED_BYTE, rawTexture.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	return textureBufferID;
}

GLuint ResourceManager::generateTexture(std::vector<std::string> textureFileNames) {
	int numTextures = textureFileNames.size();

	if (numTextures == 0) {
		return -1u;
	}

	int indTexDim = 16;
	int texDim = this->determineTextureGridSize(numTextures);
	int numPixels = texDim * indTexDim;

	std::vector<unsigned char> combinedTexture;
	combinedTexture.resize((numPixels) * (3 * numPixels));

	//set default colors to Magenta
	for (int texRowIdx = 0; texRowIdx < numPixels; texRowIdx++) {
		for (int texColIdx = 0; texColIdx < numPixels; texColIdx++) {
			int idx = texRowIdx * (numPixels * 3) + texColIdx * 3;

			combinedTexture.at(idx) = 250;
			combinedTexture.at(idx + 1) = 0;
			combinedTexture.at(idx + 2) = 255;
		}
	}

	for (unsigned textureFileIdx = 0; textureFileIdx < textureFileNames.size(); textureFileIdx++) {
		if (SDL_Surface* textureData = getSDLSurface(textureFileNames.at(textureFileIdx))) {
			int startCol = (textureFileIdx % texDim) * indTexDim;
			int startRow = (textureFileIdx / texDim) * indTexDim;

			for (int texRow = 0; texRow < indTexDim; texRow++)
			{
				unsigned char *rawData = (unsigned char *)textureData->pixels;

				std::copy(&rawData[3 * texRow * indTexDim],
						&rawData[3 * (texRow + 1) * indTexDim],
						combinedTexture.begin() + (startRow + texRow) * numPixels * 3 + startCol * 3);
			}

			SDL_FreeSurface (textureData);
		}
	}

	GLuint textureBufferID;
	glGenTextures(1, &textureBufferID);
	glBindTexture(GL_TEXTURE_2D, textureBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, numPixels, numPixels, 0, GL_RGB, GL_UNSIGNED_BYTE, combinedTexture.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	ResourceManager::textureResourceStore.emplace(textureBufferID, 1);

	return textureBufferID;
}

void ResourceManager::setTexture(std::string textureFileName) {
	this->setTexture("gTextureSampler", textureFileName);
}

void ResourceManager::setTexture(GLuint textureID) {
	this->setTexture("gSampler", textureID);
}

void ResourceManager::setTexture(std::string shaderSamplerName, std::string textureFileName) {
	if (ResourceManager::textureResourceID.count(textureFileName) == 0) {
		return;
	}

	this->setTexture(shaderSamplerName, this->textureResourceID.at(textureFileName));
}

void ResourceManager::setTexture(std::string shaderSamplerName, GLuint textureID) {
	this->getShaderManager().bindTexture(shaderSamplerName, textureID);
}

void ResourceManager::unloadTexture(std::string textureFileName) {
	if (ResourceManager::textureResourceID.count(textureFileName) == 0) {
		return;
	}

	GLuint bufferID = ResourceManager::textureResourceID.at(textureFileName);
	assert (ResourceManager::textureResourceStore.count(bufferID) != 0);

	this->unloadTexture(bufferID);

	if (ResourceManager::textureResourceStore.count(bufferID) == 0) {
		ResourceManager::textureResourceID.erase(textureFileName);
	}
}

void ResourceManager::unloadTexture(GLuint textureID) {
	if ((--ResourceManager::textureResourceStore.at(textureID)) == 0) {
		ResourceManager::textureResourceStore.erase(textureID);
		glDeleteTextures(1, &textureID);
	}
}

void ResourceManager::FinaliseResourceManager(void) {
	if (this->errorTexture != -1u) {
		glDeleteTextures(1, &this->errorTexture);
		this->errorTexture = -1u;
	}
}

void ResourceManager::setupShaderDataFormat(void) {
	GLuint VertexArrayID;
	glGenVertexArrays (1, &VertexArrayID);
	glBindVertexArray (VertexArrayID);
}

// removeShaderDataFormat stuff

ResourceManager::~ResourceManager() {}
