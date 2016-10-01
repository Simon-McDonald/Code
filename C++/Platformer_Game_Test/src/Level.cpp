/*
 * Level.cpp
 *
 *  Created on: Sep 11, 2016
 *      Author: Simon
 */

#include <array>

#include "Level.h"

#include "CheckErrors.h"

Level::Level(std::string configInfo) :
	levelConfigInfo(configInfo) {

	worldArray.resize(16);
	for (std::vector<std::vector<char>>::iterator itr = this->worldArray.begin();
			itr != this->worldArray.end();
			++itr) {
		itr->resize(10);
	}
}

bool Level::LoadResources(void) {
	const int width = 16;
	const int height = 10;

	char worldTemplate[width][height + 1] = {
			"BBBBBBBBBB",
			"B    C   B",
			"B        B",
			"BC       B",
			"BllL     B",
			"BrrR     B",
			"B        B",
			"BlL      B",
			"BrR      B",
			"B     C  B",
			"B        B",
			"B        B",
			"B    C   B",
			"B        B",
			"B        B",
			"BBBBBBBBBB"
	};

	std::map<std::string, int> blockIndexMap;
	std::vector<std::pair<std::string, std::vector<GLfloat>>> bla;

	for (int widthIdx = 0, heightIdx = 0;
			widthIdx < width;
			widthIdx += (heightIdx + 1 == height) ? 1 : 0,
			heightIdx = (heightIdx + 1) % height) {

		std::string imageName = "";
		char imageID = worldTemplate[widthIdx][heightIdx];

		switch (imageID) {
		case 'B':
			imageName = "brick.png";
			break;
		case 'C':
			imageName = "chance.png";
			break;
		case 'l':
			imageName = "pipe-left.png";
			break;
		case 'r':
			imageName = "pipe-right.png";
			break;
		case 'L':
			imageName = "pipe-left-top.png";
			break;
		case 'R':
			imageName = "pipe-right-top.png";
			break;
		default:
			imageID = ' ';
			break;
		}

		worldArray.at(widthIdx).at(heightIdx) = imageID;

		if (imageID == ' ') {
			continue;
		}

		if (blockIndexMap.count(imageName) == 0) {
			blockIndexMap.emplace(imageName, bla.size());
			bla.push_back({imageName, {}});
		}

		int blaIdx = blockIndexMap.at(imageName);

		bla.at(blaIdx).second.push_back((float) widthIdx);
		bla.at(blaIdx).second.push_back((float) heightIdx);
	}


	/*std::vector<std::pair<std::string, std::vector<GLfloat>>> bla = {
		std::make_pair<std::string, std::vector<GLfloat>>("brick.png", {0, 0, 1, 0, 2, 0, 3, 0, 4, 0, 5, 0, 6, 0, 7, 0, 8, 0, 9, 0}),
		std::make_pair<std::string, std::vector<GLfloat>>("chance.png", {0, 1}),
		std::make_pair<std::string, std::vector<GLfloat>>("pipe-left.png", {3, 1, 6, 1, 6, 2}),
		std::make_pair<std::string, std::vector<GLfloat>>("pipe-right.png", {4, 1, 7, 1, 7, 2}),
		std::make_pair<std::string, std::vector<GLfloat>>("pipe-left-top.png", {3, 2, 6, 3}),
		std::make_pair<std::string, std::vector<GLfloat>>("pipe-right-top.png", {4, 2, 7, 3}),
	};*/

	this->sceneObjects.InitialiseBillboard(bla);

	std::vector<std::string> foo = {
		"player-stationary.png",
		"player-move1.png",
		"player-move2.png",
		"player-move3.png"
	};

	this->player.InitialisePlayer(foo);

	return true;
}

bool Level::InitialiseLevel(Camera &camera) {
	this->getWindow().setClearColour(0.47f, 0.71f, 1.0f, 1.0f);

	camera.Set(glm::vec2(7, 4));

	//std::vector<G>








    // Create a Vertex Buffer Object and copy the vertex data to it
    //GLuint vbo;
    /*glGenBuffers(1, &vbo);

    GLfloat vertices[] = {
         0.0f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/


    //imageID = this->loadTexture("chance.png");


	return true;
}

bool Level::UpdateLevel(float timeIncrement, Camera &camera) {
	this->player.Update(timeIncrement, this->worldArray);

	return true;
}

void Level::RenderLevel(Camera &camera) {
	this->getShaderManager("Billboard").useProgram();
	this->getShaderManager().enableAttributeLocations();

	camera.SetViewMatrix();
	this->sceneObjects.Render();

	CHECKERRORS();

	this->getShaderManager().disableAttributeLocations();
	CHECKERRORS();





	this->getShaderManager("Character").useProgram();
	this->getShaderManager().enableAttributeLocations();

	camera.SetViewMatrix();
	this->player.Render();

	this->getShaderManager().disableAttributeLocations();
	CHECKERRORS();
}

void Level::FinaliseLevel(void) {

}

Level::~Level(void) {

}
