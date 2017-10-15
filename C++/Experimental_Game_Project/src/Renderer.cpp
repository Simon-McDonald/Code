/*
 * Renderer.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: Simon
 */

/*#include <stddef.h>
#include "Renderer.h"
#include <cstdlib>
#include <stdio.h>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "CheckErrors.h"

Renderer::Renderer() : vao(0), vbo(0), ebo(0), pointArray(NULL), pointCounter(0), polygonArray(NULL), polygonCounter(0), geometryShader(NULL) {}

int Renderer::addToRenderList(RenderableObject *object) {
	objectsList.push_back(object);
	pointOffsetList.push_back(this->pointCounter);
	polygonOffsetList.push_back(this->polygonCounter);

	this->pointCounter += object->getNumberPoints();
	this->polygonCounter += object->getNumberPolygons();

	return objectsList.size() - 1;
}

void Renderer::Initialise(GeometryShader *geometryShader) {
	this->geometryShader = geometryShader;

	this->pointArray = new point[pointCounter];
	this->polygonArray = new polygon[polygonCounter];

	for (int index = 0, limit = objectsList.size(); index < limit; index++) {
		objectsList.at(index)->populateGeometry(&this->pointArray[this->pointOffsetList[index]],
												&this->polygonArray[this->polygonOffsetList[index]]);

		int offset = this->pointOffsetList[index];
		int numberPolygons = objectsList.at(index)->getNumberPolygons();

		std::cout << "THE NUMBER OF POLYGONS: " << numberPolygons << std::endl;

		for (int polygonIndex = 0; polygonIndex < numberPolygons; polygonIndex++) {
			this->polygonArray[this->polygonOffsetList[index] + polygonIndex].p1 += offset;
			this->polygonArray[this->polygonOffsetList[index] + polygonIndex].p2 += offset;
			this->polygonArray[this->polygonOffsetList[index] + polygonIndex].p3 += offset;
		}
	}

	std::cout << "Point Array:" << std::endl;
	for (GLuint index = 0; index < pointCounter; index++) {
		std::cout << "(" << this->pointArray[index].x << ", " << this->pointArray[index].y << ", " << this->pointArray[index].z << ", " << this->pointArray[index].r << ", "
				<< this->pointArray[index].g << ", " << this->pointArray[index].b << ")" << std::endl;
	}
	std::cout << std::endl;

	std::cout << "Indices Array:" << std::endl;
	for (GLuint index = 0; index < polygonCounter; index++) {
		std::cout << "(" << this->polygonArray[index].p1 << ", " << this->polygonArray[index].p2 << ", " << this->polygonArray[index].p3 << ")" << std::endl;
	}
	std::cout << std::endl;

	CHECKERRORS();

	glGenVertexArrays(1, &this->vao);
	glBindVertexArray(this->vao);

	glGenBuffers(1, &this->vbo);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, (this->pointCounter * sizeof (GLfloat) * (numDims + numCols)), (GLfloat*) this->pointArray, GL_STATIC_DRAW);

	glGenBuffers(1, &this->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, (this->polygonCounter * sizeof (GLuint) * 3), (GLuint*) this->polygonArray, GL_STATIC_DRAW);

    GLint posAttrib = glGetAttribLocation(this->geometryShader->getShaderProgram(), "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, numDims, GL_FLOAT, GL_FALSE, (numDims + numCols) * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(this->geometryShader->getShaderProgram(), "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, numCols, GL_FLOAT, GL_FALSE, (numDims + numCols) * sizeof(GLfloat), (void*)(numDims * sizeof(GLfloat)));
}

void Renderer::Render(int objectIndex, glm::mat4 model) {
    glUniformMatrix4fv(this->geometryShader->getUniModel(), 1, GL_FALSE, glm::value_ptr(model));

    GLuint startIndex = polygonOffsetList.at(objectIndex);
    GLuint numPolygons = objectsList.at(objectIndex)->getNumberPolygons();

	glDrawElements(GL_TRIANGLES, numPolygons * 3, GL_UNSIGNED_INT, (void*) (startIndex * 3 * sizeof(GLfloat)));
}

void Renderer::Destroy() {
    glDeleteBuffers(1, &this->ebo);
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

Renderer::~Renderer() {
	delete[] this->pointArray;
	delete[] this->polygonArray;
}*/
