/*
 * Level.cpp
 *
 *  Created on: Sep 11, 2016
 *      Author: Simon
 */

#include <array>

#include "Level.h"

Level::Level(std::string configInfo) :
	levelConfigInfo(configInfo) {
}

bool Level::LoadResources(void) {
	/*std::array<std::string, 2> textureResourcesList = {
			"brick.png", "chance.png"
	};

	for (std::array<std::string, 2>::iterator arrItr = textureResourcesList.begin();
			arrItr != textureResourcesList.end(); arrItr++) {
		this->loadTexture(*arrItr);
	}*/

	this->generateTexture("TestTexture1", {"brick.png", "chance.png"});

	return true;
}

bool Level::InitialiseLevel(void) {
	this->getWindow().setClearColour(0.47f, 0.71f, 1.0f, 1.0f);

	//std::vector<G>
	std::vector<std::pair<std::string, std::vector<GLfloat>>> bla =
	{
			std::make_pair<std::string, std::vector<GLfloat>>("brick.png", {0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, -1.0})
	};

	this->sceneObjects.InitialiseBillboard(bla);







    // Create a Vertex Buffer Object and copy the vertex data to it
    //GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {
         0.0f,  0.5f,
         0.5f, -0.5f,
        -0.5f, -0.5f
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    imageID = this->loadTexture("chance.png");


	return true;
}

bool Level::UpdateLevel(float timeIncrement, Camera &camera) {
	this->getShaderManager().enableAttributeLocations();



    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    this->setTexture(imageID);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glDrawArrays(GL_TRIANGLES, 0, 3);

	//this->setTexture("TestTexture1");
	//this->sceneObjects.Render();

	this->getShaderManager().disableAttributeLocations();

	return true;
}

void Level::FinaliseLevel(void) {

}

Level::~Level(void) {

}
