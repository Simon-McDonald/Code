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

	static bool InitialiseResourceManager(void);

	SDL_Surface* getSDLSurface(std::string imageFileName);

	GLuint loadTexture(std::string textureFileName);

	static GLuint generateErrorTexture(void);
	GLuint generateTexture(std::vector<std::string> textureFileNames);
	int determineTextureGridSize(int numTextures);

	void unloadTexture(std::string textureFileName);
	void unloadTexture(GLuint textureID);

	void setTexture(std::string textureFileName);
	void setTexture(GLuint textureID);
	void setTexture(std::string shaderSamplerName, std::string textureFileName);
	void setTexture(std::string shaderSamplerName, GLuint textureID);

	void FinaliseResourceManager(void);

	static void setupShaderDataFormat(void);

	~ResourceManager(void);

private:
	/*
	 * Map between the image name used to generate the texture and the buffer ID.
	 */
	static std::map<std::string, GLuint> textureResourceID;

	/*
	 * Map between the texture buffers ID and the number of current references.
	 */
	static std::map<GLuint, unsigned> textureResourceStore;

	static GLuint errorTexture;
};

#endif /* RESOURCEMANAGER_H_ */
