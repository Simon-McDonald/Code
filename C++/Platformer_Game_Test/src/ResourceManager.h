/*
 * ObjLoader.h
 *
 *  Created on: Mar 25, 2016
 *      Author: Simon
 */

#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <array>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "UtilityManager.h"
#include "WorldManager.h"

class ResourceManager : protected WorldManager, protected UtilityManager {
public:
	ResourceManager(void);

	bool InitialiseResourceManager(void);

	SDL_Surface* getSDLSurface(std::string imageFileName);

	GLuint loadTexture(std::string textureFileName);

	GLuint generateTexture(std::vector<std::string> textureFileNames);
	int determineTextureGridSize(int numTextures);

	void unloadTexture(std::string textureFileName);

	void setTexture(std::string textureFileName);
	void setTexture(GLuint textureID);
	void setTexture(std::string shaderSamplerName,
			std::string textureFileName);

	void FinaliseResourceManager(void);

	static void setupShaderDataFormat(void);

	~ResourceManager(void);

private:
	static std::map<std::string, std::pair<GLuint, unsigned>> textureResourceID;
	static std::map<GLuint, unsigned> textureResourceStore;
};

#endif /* RESOURCEMANAGER_H_ */
