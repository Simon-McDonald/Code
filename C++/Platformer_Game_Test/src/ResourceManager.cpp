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

#include "ShaderManager.h"

#include <iostream>
#include "ResourceManager.h"

std::map<std::string, std::pair<GLuint, unsigned>> ResourceManager::textureResourceID;

ResourceManager::ResourceManager() {}

bool ResourceManager::InitialiseResourceManager(void) {
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
		// texture has already been loaded, need to increment counter
		return -1u;
	}

	/*std::string texturePath = this->getConfig().getString("RESOURCES", "images_dir") + "\\" + textureFileName;

	SDL_Surface *textureData = IMG_Load(texturePath.c_str());
	if (!textureData) {
		ERR << "Failed to load image: '" << texturePath << "'" << std::endl;
		return;
	} else {
		INFO << "Loaded image '" << texturePath << "'" << std::endl;
	}*/

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

		ResourceManager::textureResourceID.emplace(textureFileName, std::make_pair(textureBufferID, 1));

		return textureBufferID;
	}

	return -1u;
} /* ResourceManager::loadTexture */

void ResourceManager::generateTexture(std::string stringIdentifier, std::vector<std::string> textureFileNames) {
	int numTextures = textureFileNames.size();

	if (numTextures == 0) {
		return;
	}

	int indTexDim = 16;
	int texDim = static_cast<int>(ceil(sqrt(static_cast<float>(numTextures))));
	int numPixels = texDim * indTexDim;

	std::vector<unsigned char> combinedTexture;
	combinedTexture.resize(((numPixels) * (numPixels + 1)) * 3);
	//set default colors to Magenta

	for (int texRowIdx = 0; texRowIdx < numPixels; texRowIdx++) {
		for (int texColIdx = 0; texColIdx < numPixels; texColIdx += 3) {
			int idx = texRowIdx * (numPixels + 1) + texColIdx;

			combinedTexture.at(idx) = 255;
			combinedTexture.at(idx + 1) = 0;
			combinedTexture.at(idx + 2) = 255;
		}
	}

	for (unsigned textureFileIdx = 0; textureFileIdx < textureFileNames.size(); textureFileIdx++) {
		if (SDL_Surface* textureData = getSDLSurface(textureFileNames.at(textureFileIdx))) {
			int startCol = (textureFileIdx % texDim) * indTexDim;
			int startRow = (textureFileIdx / texDim) * indTexDim;

			for (int texRow = 0; texRow < texDim; texRow++)
			{
				unsigned char *rawData = (unsigned char *)textureData->pixels;

				std::copy(&rawData[3 * texRow * (indTexDim + 1)],
						&rawData[3 * (texRow + 1) * (indTexDim + 1) - 1],
						combinedTexture.begin() + (startRow + texRow) * numPixels + startCol);
			}

			SDL_FreeSurface (textureData);
		}
	}

	GLuint textureBufferID;
	glGenTextures(1, &textureBufferID);
	glBindTexture(GL_TEXTURE_2D, textureBufferID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, numPixels, numPixels, 0, GL_BGR, GL_UNSIGNED_BYTE, combinedTexture.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	ResourceManager::textureResourceID.emplace(stringIdentifier, std::make_pair(textureBufferID, 1));
}

void ResourceManager::setTexture(std::string textureFileName) {
	if (ResourceManager::textureResourceID.count(textureFileName) == 0) {
		return;
	}



	//this->getShaderManager().bindTexture("gTextureSampler", this->textureResourceID.at(textureFileName));
}

void ResourceManager::setTexture(GLuint textureID) {
	this->getShaderManager().bindTexture("gSampler", textureID);
}

void ResourceManager::setTexture(std::string shaderSamplerName, std::string textureFileName) {
	if (ResourceManager::textureResourceID.count(textureFileName) == 0) {
		return;
	}

	this->getShaderManager().bindTexture(shaderSamplerName, this->textureResourceID.at(textureFileName).first);
}

void ResourceManager::FinaliseResourceManager(void) {

}

void ResourceManager::setupShaderDataFormat(void) {
	GLuint VertexArrayID;
	glGenVertexArrays (1, &VertexArrayID);
	glBindVertexArray (VertexArrayID);
}

ResourceManager::~ResourceManager() {}
