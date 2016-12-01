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

#include <SDL_image.h>
#include <SOIL.h>

#include "ShaderManager.h"
#include "CheckErrors.h"

#include <iostream>
#include "ResourceManager.h"

/*ObjectDimensions loadBMP_raw(const char * imagepath, std::vector<unsigned char> &dataVector) {
	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file)
	{printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return {0, 0};}
	else
	{printf ("Got the file open\n");}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){
		printf("Not a correct BMP file\n");
		return {0, 0};
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file, no header\n");
		return {0, 0};
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return {0, 0};}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return {0, 0};}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	// Everything is in memory now, the file wan be closed
	fclose (file);

	dataVector.resize(imageSize);
	memcpy(dataVector.data(), data, imageSize);

	// OpenGL has now copied the data. Free our own version
	delete [] data;
	return {width, height};
}

GLuint loadBMP_custom(const char * imagepath){

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file)
	{printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0;}
	else
	{printf ("Got the file open\n");}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){
		printf("Not a correct BMP file\n");
		return 0;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file, no header\n");
		return 0;
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file\n");    return 0;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file\n");    return 0;}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	// Everything is in memory now, the file wan be closed
	fclose (file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete [] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// ... nice trilinear filtering.
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}*/

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

std::map<std::string, struct ResourceManager::bufferedResource> ResourceManager::bufferedResourceID;
std::map<std::string, GLuint> ResourceManager::textureResourceID;

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

		/*int width;
		int height;
		void *imageData;*/

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

void ResourceManager::loadRawImage(std::string imageFileName, int &width, int&height, void *&imageData) {
	std::string texturePath = this->getConfig().getString("RESOURCES", "images_dir") + "\\" + imageFileName;

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
	//imageData = new unsigned char[width * height * 3 + height];
	//memcpy(imageData, textureData->pixels, width * height * 3 + height);

	/*unsigned char * dataPtr = (unsigned char *) imageData;
	unsigned char * tempPtr = (unsigned char *) textureData->pixels;

	for (int heightIdx = 0; heightIdx < height; heightIdx++)
	{
		int srcSt = heightIdx * width * 3;// + heightIdx;
		int destSt = heightIdx * width * 3;
		int len = width * 3;
		memcpy (&dataPtr[destSt], &tempPtr[srcSt], len);
		INFO << "Source index, start: " << srcSt << " - Destination index, start: " << destSt << " - Length: " << len << std::endl;
	}*/



	/*unsigned char *data = (unsigned char *) textureData->pixels;
	if (imageFileName == "smallRed2.bmp") {
		printf ("Printing the contents of blue.bmp\n");
		for (int i = 0; i < textureData->w * textureData->h * 3; i+=3){
			printf ("(%i, %i, %i)-", data[i], data[i+1], data[i+2]);
		}
	}*/

	//SDL_FreeSurface (textureData);
} /* ResourceManager::loadRawImage */

void ResourceManager::loadTexture(std::string textureFileName) {
	if (ResourceManager::textureResourceID.count(textureFileName) != 0) {
		// texture has already been loaded!
		return;
	}

	std::string texturePath = this->getConfig().getString("RESOURCES", "images_dir") + "\\" + textureFileName;

	SDL_Surface *textureData = IMG_Load(texturePath.c_str());
	if (!textureData) {
		ERR << "Failed to load image: '" << texturePath << "'" << std::endl;
		return;
	} else {
		INFO << "Loaded image '" << texturePath << "'" << std::endl;
	}

	GLuint textureBufferID;
	glGenTextures(1, &textureBufferID);

	glBindTexture(GL_TEXTURE_2D, textureBufferID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData->w, textureData->h, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData->pixels);

	unsigned char *data = (unsigned char *) textureData->pixels;
	if (textureFileName == "smallRed.bmp") {
		printf ("Printing the contents of blue.bmp\n");
		for (int i = 0; i < textureData->w * textureData->h; i+=600){
			printf ("(%i, %i, %i)-", data[i], data[i+1], data[i+2]);
		}
	}

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

	struct bufferedResource br = {vertexbuffer, uvbuffer, normalbuffer, vertices.size()};

	ResourceManager::bufferedResourceID.emplace(meshFileName, br);
}

void ResourceManager::setupShaderDataFormat(void) {
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

	struct bufferedResource *meshIDs = &ResourceManager::bufferedResourceID.at(meshFileName);
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
