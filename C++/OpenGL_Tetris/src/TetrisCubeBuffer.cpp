/*
 * TetrisCubeBuffer.cpp
 *
 *  Created on: May 7, 2017
 *      Author: Simon
 */

/*#include "DataBuffer.hpp"
#include "TetrisCubeBuffer.h"

typedef struct {
	GLuint dataBuffers[3];
	GLuint indexBuffer;
	GLuint texture;
	GLuint vertexBuffer;
	unsigned numVertices;
} VertexResource;

TetrisCubeBuffer::TetrisCubeBuffer(std::string pieceDefinition, glm::vec3 colourVec) {



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
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexResource.dataBuffers[2]);
	glBufferData(GL_ARRAY_BUFFER, normalsList.size() * sizeof(glm::vec3), normalsList.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexResource.indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indicesList.size(), indicesList.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);

	vertexResource.texture = this->loadTexture(textureFile);
}*/


