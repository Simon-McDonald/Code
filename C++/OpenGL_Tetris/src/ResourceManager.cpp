/*
 * ObjLoader.cpp
 *
 *  Created on: Mar 25, 2016
 *      Author: Simon
 */

#include <iostream>
#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include "CheckErrors.h"

#include <iostream>
#include "ResourceManager.h"


bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
){
	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if( file == NULL ){
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
		getchar();
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices    .push_back(uvIndex[0]);
			uvIndices    .push_back(uvIndex[1]);
			uvIndices    .push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}else{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ){

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs     .push_back(uv);
		out_normals .push_back(normal);
	}

	return true;
}

bool ResourceManager::loadObjectFile(
	std::string fileName,
	std::vector<glm::vec3> &verticesList,
	std::vector<glm::vec2> &uvsList,
	std::vector<glm::vec3> &normalsList,
	std::vector<GLuint> &indicesList,
	std::string &textureFile) {

	INFO << "Load " << fileName << END;

	std::vector<unsigned> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> tempVertices;
	std::vector<glm::vec2> tempUVs;
	std::vector<glm::vec3> tempNormals;

	FILE *file = fopen(fileName.c_str(), "r");
	if (file == NULL) {
		ERR << "Cannot open file " << fileName << END;
		return false;
	}

	while(true) {
		char lineHeader[128];

		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {
			break;
		}

		if (strcmp(lineHeader, "v") == 0){
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			tempVertices.push_back(vertex);
		} else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y;
			tempUVs.push_back(uv);
		} else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			tempNormals.push_back(normal);
		} else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );

			if (matches != 9) {
				ERR << "File can't be read by our simple parser" << END;
				return false;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		} else if (strcmp(lineHeader, "usemtl") == 0) {
			char imageName[256];
			fscanf(file, "%s\n", imageName);
			textureFile = std::string(imageName);
		} else {
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}
	}

	for(unsigned int i=0; i < vertexIndices.size(); i++){
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 vertex = tempVertices[vertexIndex - 1];
		glm::vec2 uv = tempUVs[uvIndex - 1];
		glm::vec3 normal = tempNormals[normalIndex - 1];

		verticesList.push_back(vertex);
		uvsList.push_back(uv);
		normalsList.push_back(normal);
	}

	indicesList.resize(vertexIndices.size());
	for (unsigned idx = 0; idx < indicesList.size(); ++idx) {
		indicesList.at(idx) = idx;
	}

	return true;
}

void ResourceManager::loadObjectIntoVertexBuffer(std::string objectFilename) {
/* typedef struct {
	GLuint dataBuffers[3];
	GLuint indexBuffer;
	GLuint texture;
	GLuint vertexBuffer;
} vertexResource;

static std::map<std::string, vertexResource> vertexResourceMap; */

	std::string objectFilePath = this->getConfig().getString("RESOURCES", "meshes_dir") + "\\" + objectFilename;

	std::vector<glm::vec3> verticesList;
	std::vector<glm::vec2> uvsList;
	std::vector<glm::vec3> normalsList;
	std::vector<unsigned> indicesList;
	std::string textureFile;

	if (!this->loadObjectFile(objectFilePath, verticesList, uvsList, normalsList, indicesList, textureFile)) {
		ERR << "Some error with " << objectFilename << END;
		return;
	}

	VertexResource vertexResource;

	vertexResource.numVertices = indicesList.size() / 3;

	glGenVertexArrays(1, &vertexResource.vertexBuffer);
	glBindVertexArray(vertexResource.vertexBuffer);

	glGenBuffers(3, vertexResource.dataBuffers);
	glGenBuffers(1, &vertexResource.indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexResource.dataBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, verticesList.size() * sizeof(glm::vec3), verticesList.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexResource.dataBuffers[1]);
	glBufferData(GL_ARRAY_BUFFER, uvsList.size() * sizeof(glm::vec2), uvsList.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexResource.dataBuffers[2]);
	glBufferData(GL_ARRAY_BUFFER, normalsList.size() * sizeof(glm::vec3), normalsList.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexResource.indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indicesList.size(), indicesList.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	vertexResource.texture = this->loadTexture(textureFile);

	ResourceManager::vertexResourceMap.emplace(objectFilename, vertexResource);
}

void ResourceManager::renderVertexBufferReference(std::string objectFilename) {
	if (!ResourceManager::vertexResourceMap.count(objectFilename)) {
		ERR << "Somethign blal" << END;
		return;
	}

	VertexResource &vertexResource = ResourceManager::vertexResourceMap.at(objectFilename);

	glBindVertexArray(vertexResource.vertexBuffer);

	this->setTexture(vertexResource.texture);

	glDrawElementsBaseVertex(GL_TRIANGLES, vertexResource.numVertices, GL_UNSIGNED_INT, 0, 0);

	glBindVertexArray(0);
}

std::map<std::string, ResourceManager::BufferedResource> ResourceManager::bufferedResourceID;
std::map<std::string, GLuint> ResourceManager::textureResourceID;
std::map<std::string, ResourceManager::VertexResource> ResourceManager::vertexResourceMap;

ResourceManager::ResourceManager() {}
ResourceManager::~ResourceManager() {}

bool ResourceManager::InitialiseResourceManager(void) {
	return true;
}

void ResourceManager::loadCubeTexture(std::array<std::string, 6> &fileNames,
		                              std::string cubeName) {
	if (fileNames.size() != 6) {
		ERR << "Attempting to load a cube map texture " << cubeName
			<< " but missing some textures" << std::endl;
		return;
	}

	std::string imageDirPath = this->getConfig().getString("RESOURCES", "images_dir") + "\\";

	static const GLenum types[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	                                  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	                                  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

	GLuint cubeMapID;
	glGenTextures(1, &cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);

	for (unsigned int arrIdx = 0; arrIdx < fileNames.size(); arrIdx++) {
		std::string imageLoc = imageDirPath + fileNames.at(arrIdx);
		SDL_Surface *textureData = IMG_Load(imageLoc.c_str());
		if (!textureData) {
			ERR << "Failed to load image: '" << imageLoc << "', used for cube texture: '" << cubeName << "'" << std::endl;
			return;
		} else {
			DEBUG << "Loaded image: '" << imageLoc << "', used for cube texture: '" << cubeName << "', size (w, h): ("
				<< textureData->w << ", " << textureData->h << ")" << std::endl;
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

    ResourceManager::textureResourceID.emplace(cubeName, cubeMapID);
} /* ResourceManager::loadCubeTexture */

void ResourceManager::loadRawImage(std::string imageFileName, int &width, int &height, void *&imageData) {
	std::string texturePath = ResourceManager::getConfig().getString("RESOURCES", "images_dir") + "\\" + imageFileName;

	SDL_Surface *textureData = IMG_Load(texturePath.c_str());
	if (!textureData) {
		ERR << "Failed to load image: '" << texturePath << "'" << std::endl;
		return;
	} else {
		INFO << "Loaded image '" << texturePath << "'" << std::endl;
	}

	width = textureData->w;
	height = textureData->h;
	imageData = textureData->pixels;
} /* ResourceManager::loadRawImage */

SDL_Surface* ResourceManager::loadRawImage(std::string imageFileName) {
	std::string texturePath = ResourceManager::getConfig().getString("RESOURCES", "images_dir") + "\\" + imageFileName;

	SDL_Surface *textureData = IMG_Load(texturePath.c_str());
	if (!textureData) {
		ERR << "Failed to load image: '" << texturePath << "'" << std::endl;
		return nullptr;
	} else {
		INFO << "Loaded image '" << texturePath << "'" << std::endl;
	}

	return textureData;
} /* ResourceManager::loadRawImage */

GLuint ResourceManager::dataToTexture(std::string textureFileName, SDL_Surface *textureData) {
	GLuint textureBufferID;
	glGenTextures(1, &textureBufferID);

	glBindTexture(GL_TEXTURE_2D, textureBufferID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->w, textureData->h, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData->pixels);

	SDL_FreeSurface (textureData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_LINEAR, GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR, GL_NEAREST
	// For pixelated images
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	ResourceManager::textureResourceID.emplace(textureFileName, textureBufferID);
	return textureBufferID;
}

GLuint ResourceManager::loadTexture(const std::string &textureFileName) {
	if (ResourceManager::textureResourceID.count(textureFileName) != 0) {
		// texture has already been loaded!
		return -1u;
	}

	std::string texturePath = ResourceManager::getConfig().getString("RESOURCES", "images_dir") + "\\" + textureFileName;

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
	// For pixelated images
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	ResourceManager::textureResourceID.emplace(textureFileName, textureBufferID);
	return textureBufferID;
} /* ResourceManager::loadTexture */

void ResourceManager::loadMesh(std::string meshFileName) {
	if (ResourceManager::bufferedResourceID.count(meshFileName) != 0) {
		// mesh has already been loaded!
		return;
	}

	std::string meshPath = this->getConfig().getString("RESOURCES", "meshes_dir") + "\\" + meshFileName;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	if (!loadOBJ(meshPath.c_str(), vertices, uvs, normals)) {
		std::cout << "Failed to load mesh " << meshFileName << std::endl;
	}

	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	BufferedResource br = {vertexbuffer, uvbuffer, normalbuffer, vertices.size()};

	ResourceManager::bufferedResourceID.emplace(meshFileName, br);
}

void ResourceManager::setupShaderDataFormat(void) {
	INFO << "GENERATE VERTEX BUFFERS FIRST!" << END;

	GLuint VertexArrayID;
	glGenVertexArrays (1, &VertexArrayID);
	glBindVertexArray (VertexArrayID);
}

void ResourceManager::setTexture(std::string textureFileName) {
	if (ResourceManager::textureResourceID.count(textureFileName) == 0) {
		ERR << "Did not load image: '" << textureFileName << "'" << std::endl;
		return;
	}

	this->getShaderManager().bindTexture("gSampler", this->textureResourceID.at(textureFileName));
}

void ResourceManager::setTexture(std::string shaderSamplerName, std::string textureFileName) {
	if (ResourceManager::textureResourceID.count(textureFileName) == 0) {
		return;
	}

	this->getShaderManager().bindTexture(shaderSamplerName, this->textureResourceID.at(textureFileName));
}

void ResourceManager::setCubeTexture(std::string shaderSamplerName, std::string textureCubeName) {
	if (ResourceManager::textureResourceID.count(textureCubeName) == 0) {
		return;
	}

	this->getShaderManager().bindTexture(shaderSamplerName, this->textureResourceID.at(textureCubeName), GL_TEXTURE_CUBE_MAP);
} /* ResourceManager::setCubeTexture */

/*void SkyMapManager::bindSkyMap(std::string samplerName) {
	//INFO << "Bind the skyMap to texture name: " << samplerName << std::endl;
	this->getShaderManager().getActiveShaderManager()->bindTexture(samplerName, this->textureBufferID, GL_TEXTURE_CUBE_MAP);
}*/

void ResourceManager::setTexture(GLuint textureID) {
	this->getShaderManager().bindTexture("gSampler", textureID);
}

void ResourceManager::renderMesh(std::string meshFileName) {
	this->renderMesh(meshFileName, false);
}

void ResourceManager::renderMesh(std::string meshFileName, bool renderFlag) {
	if (ResourceManager::bufferedResourceID.count(meshFileName) == 0) {
		WARN << "Non-existant mesh" << std::endl;
		return;
	}

	BufferedResource *meshIDs = &ResourceManager::bufferedResourceID.at(meshFileName);
	CHECKERRORS();
	glBindBuffer(GL_ARRAY_BUFFER, meshIDs->vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	CHECKERRORS();
	glBindBuffer(GL_ARRAY_BUFFER, meshIDs->uvbuffer);
	glVertexAttribPointer(
		1,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	CHECKERRORS();
	glBindBuffer(GL_ARRAY_BUFFER, meshIDs->normalbuffer);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	CHECKERRORS();
	ShaderManager::getActiveShaderManager()->validateProgram();

	if (renderFlag) {
		glDrawArrays(GL_PATCHES, 0, meshIDs->verticesSize);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, meshIDs->verticesSize);
	}
}
