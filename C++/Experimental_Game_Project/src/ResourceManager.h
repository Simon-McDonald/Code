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

#include <UtilityManager.hpp>
#include <WorldManager.hpp>

class ResourceManager : protected WorldManager, protected UtilityManager {
public:
	ResourceManager(void);

	bool InitialiseResourceManager(void);

	void loadCubeTexture(std::array<std::string, 6> &fileNames, std::string cubeName);
	void loadRawImage(std::string imageFileName, int &width, int &height,
			void *&imageData);
	GLuint loadTexture(std::string textureFileName);
	void loadMesh(std::string meshFileName);

	void setupShaderDataFormat(void);

	void setTexture(std::string textureFileName);
	void setTexture(GLuint textureID);
	void setTexture(std::string shaderSamplerName,
			std::string textureFileName);
	void setCubeTexture(std::string shaderSamplerName, std::string textureCubeName);

	void loadObjectIntoVertexBuffer(std::string objectFilename);
	void renderVertexBufferReference(std::string objectFilename);

	void renderMesh(std::string meshFileName, bool renderFlag);
	void renderMesh(std::string meshFileName);

	~ResourceManager(void);

private:
	typedef struct {
		GLuint vertexbuffer;
		GLuint uvbuffer;
		GLuint normalbuffer;
		GLuint verticesSize;
	} BufferedResource;

	typedef struct {
		GLuint dataBuffers[3];
		GLuint indexBuffer;
		GLuint texture;
		GLuint vertexBuffer;
		unsigned numVertices;
	} VertexResource;

	static std::map<std::string, BufferedResource> bufferedResourceID;
	static std::map<std::string, GLuint> textureResourceID;
	static std::map<std::string, VertexResource> vertexResourceMap;

	bool loadObjectFile(
		std::string fileName,
		std::vector<glm::vec3> &verticesList,
		std::vector<glm::vec2> &uvsList,
		std::vector<glm::vec3> &normalsList,
		std::vector<GLuint> &indicesList,
		std::string &textureFile);
};

#endif /* RESOURCEMANAGER_H_ */
