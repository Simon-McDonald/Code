/*
 * Level.h
 *
 *  Created on: Sep 11, 2016
 *      Author: Simon
 */

#ifndef LEVEL_H_
#define LEVEL_H_

#include "Camera.h"
#include "BillboardManager.h"
#include "Player.h"
#include "ResourceManager.h"

class Level : ResourceManager {
public:
	Level(Config::ConfigHeader configInfo);

	bool LoadResources(void);

	bool InitialiseLevel(Camera &camera);

	bool UpdateLevel(float timeIncrement, Camera &camera);

	void RenderLevel(Camera &camera);

	void FinaliseLevel(void);

	~Level(void);

private:
	Config::ConfigHeader levelConfigInfo;

	BillboardManager sceneObjects;
	Player player;

	std::vector<std::vector<char>> worldArray;
	//GLuint imageID;

};

#endif /* LEVEL_H_ */
