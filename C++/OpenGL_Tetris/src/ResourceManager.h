/*
 * ObjLoader.h
 *
 *  Created on: Mar 25, 2016
 *      Author: Simon
 */

#ifndef RESOURCEMANAGER_H_
#define RESOURCEMANAGER_H_

#include <SDL_image.h>

#include <UtilityManager.hpp>
#include <WorldManager.hpp>

class ResourceManager : protected WorldManager, protected UtilityManager {
public:
    ResourceManager(void);

	static GLuint loadTexture(const std::string &textureFileName);
	static SDL_Surface* loadRawImage(std::string imageFileName);
	static GLuint dataToTexture(std::string textureFileName, SDL_Surface *textureData);

    ~ResourceManager(void);

private:
    GLuint vertexArrayID;
};

#endif /* RESOURCEMANAGER_H_ */
