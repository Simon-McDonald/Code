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

	void loadCubeTexture(std::array<std::string, 6> &fileNames, std::string cubeName);
	void loadRawImage(std::string imageFileName, int &width, int &height,
			void *&imageData);
	void loadTexture(std::string textureFileName);
	void loadMesh(std::string meshFileName);

	void setupShaderDataFormat(void);

	void setTexture(std::string textureFileName);
	void setTexture(GLuint textureID);
	void setTexture(std::string shaderSamplerName,
			std::string textureFileName);
	void setCubeTexture(std::string shaderSamplerName, std::string textureCubeName);

	void renderMesh(std::string meshFileName, bool renderFlag);
	void renderMesh(std::string meshFileName);

	~ResourceManager(void);

private:
	struct bufferedResource {
		GLuint vertexbuffer;
		GLuint uvbuffer;
		GLuint normalbuffer;
		GLuint verticesSize;
	};

	static std::map<std::string, struct bufferedResource> bufferedResourceID;
	static std::map<std::string, GLuint> textureResourceID;
};

#endif /* RESOURCEMANAGER_H_ */
