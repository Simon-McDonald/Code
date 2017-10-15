//============================================================================
// Name        : Sphere.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Sphere.h"
#include <cstdlib>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

#include "ErrorCheck.h"

void Sphere::Initialise(GeometryShader *geometryShader) {

	this->geometryShader = geometryShader;

	if (this->layers % 2 != 0) {
		std::cerr << "Sphere->Layers is not EVEN!" << std::endl;
		exit (1);
	}
	if (this->layers < 2) {
		std::cerr << "Sphere->Layers is not EVEN!" << std::endl;
		exit (1);
	}

	std::cout << "Number of layers: " << this->layers << std::endl;
	std::cout << "Number of points: " << this->numberPoints << std::endl;
	std::cout << "Number of polygons: " << this->numberPolygons << std::endl;

	GLfloat radius = 0.5f;
	GLfloat *pointIndex = this->world;

	// generate the triplet points for the vertices of the sphere
	for (int layerIndex = 0; layerIndex <= Sphere::layers; layerIndex++) {
		int layerId = (Sphere::layers/2) - abs(layerIndex - (Sphere::layers/2));
		int noPointsLayeri = noPointsAtLayeri(layerId);

		for (int layerPointIndex = 0; layerPointIndex < noPointsLayeri; layerPointIndex++) {
			GLfloat latitude = 180.0f / GLfloat(Sphere::layers) * GLfloat(layerIndex);
			GLfloat longitude = 360.0f / noPointsLayeri * layerPointIndex;
			this->addCartesian(pointIndex, latitude * M_PI / 180, longitude * M_PI / 180, radius);
			pointIndex += 3;
		}
	}
	printWorld();
	CHECKERRORS();



	CHECKERRORS();
	GLuint *triangleIndex = this->indices;

	for (GLuint layerIndex = 1; layerIndex <= (GLuint)this->layers; layerIndex++) {
		GLuint correctedLayerIndex = ((Sphere::layers + 1)/2) - abs(layerIndex - ((Sphere::layers + 1)/2));
		GLuint correctedPrevLayerIndex = ((Sphere::layers + 1)/2) - abs((layerIndex - 1) - ((Sphere::layers + 1)/2));

		GLuint upperLayerNo = 6 * correctedPrevLayerIndex;
		GLuint lowerLayerNo = 6 * correctedLayerIndex;

		GLuint upperStartIndex = (correctedPrevLayerIndex == 0) ? 0 : 3 * correctedPrevLayerIndex * correctedPrevLayerIndex - 3 * correctedPrevLayerIndex + 1;
		GLuint lowerStartIndex = (correctedLayerIndex == 0) ? 0 : 3 * correctedLayerIndex * correctedLayerIndex - 3 * correctedLayerIndex + 1;

		upperStartIndex = (correctedLayerIndex == layerIndex) ? upperStartIndex : this->numberPoints - upperStartIndex - upperLayerNo;
		lowerStartIndex = (correctedLayerIndex == layerIndex) ? lowerStartIndex : this->numberPoints - lowerStartIndex - lowerLayerNo;

		lowerStartIndex = (lowerStartIndex == (GLuint) this->numberPoints) ? this->numberPoints - 1 : lowerStartIndex;

		for (GLuint faceIndex = 0; faceIndex < 6; faceIndex++) {
			GLuint upperFaceIndex = upperStartIndex + faceIndex * upperLayerNo / 6;
			GLuint lowerFaceIndex = lowerStartIndex + faceIndex * lowerLayerNo / 6;
			bool startBottom = (correctedLayerIndex == layerIndex);

			GLuint trianglesNo = (correctedLayerIndex == layerIndex) ? (correctedLayerIndex - 1) * 2 + 1 : (correctedLayerIndex) * 2 + 1;
			for (GLuint triangleLayerIndex = 1; triangleLayerIndex <= trianglesNo; triangleLayerIndex++) {
				upperFaceIndex = (upperFaceIndex == upperStartIndex + upperLayerNo) ? upperStartIndex : upperFaceIndex;
				lowerFaceIndex = (lowerFaceIndex == lowerStartIndex + lowerLayerNo) ? lowerStartIndex : lowerFaceIndex;

				if (startBottom) {
					GLuint incLowerIndex = (lowerFaceIndex + 1 == lowerStartIndex + lowerLayerNo) ? lowerStartIndex : lowerFaceIndex + 1;
					this->addIndices(triangleIndex, upperFaceIndex, lowerFaceIndex, incLowerIndex);
					lowerFaceIndex++;
				} else {
					GLuint incUpperIndex = (upperFaceIndex + 1 == upperStartIndex + upperLayerNo) ? upperStartIndex : upperFaceIndex + 1;
					this->addIndices(triangleIndex, upperFaceIndex, incUpperIndex, lowerFaceIndex);
					upperFaceIndex++;
				}
				startBottom = !startBottom;

				triangleIndex += 3;
			}
		}
	}

	CHECKERRORS();
	printWorldIndices();
	CHECKERRORS();


	for (int index = 0; index < this->numberPolygons * 3; index++) {
		/*this->worldArray[index * 6] = this->world[this->indices[index] * 3];
		this->worldArray[index * 6 + 1] = this->world[this->indices[index] * 3 + 1];
		this->worldArray[index * 6 + 2] = this->world[this->indices[index] * 3 + 2];*/

		this->worldArray[index * 6] = this->world[index * 3];
		this->worldArray[index * 6 + 1] = this->world[index * 3 + 1];
		this->worldArray[index * 6 + 2] = this->world[index * 3 + 2];

		int tempIndex = rand()*3/RAND_MAX + 3;

		this->worldArray[index * 6 + 3] = 0.0;
		this->worldArray[index * 6 + 4] = 0.0;
		this->worldArray[index * 6 + 5] = 0.0;

		this->worldArray[index * 6 + tempIndex] = 1.0;
	}
	CHECKERRORS();

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);

    glGenBuffers(1, &this->vbo);

    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->worldArray), this->worldArray, GL_STATIC_DRAW);

    glGenBuffers(1, &this->ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->indices), this->indices, GL_STATIC_DRAW);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(this->geometryShader->getShaderProgram(), "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, this->numDims, GL_FLOAT, GL_FALSE, (this->numDims + this->numCols) * sizeof(GLfloat), 0);

    GLint colAttrib = glGetAttribLocation(this->geometryShader->getShaderProgram(), "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, this->numCols, GL_FLOAT, GL_FALSE, (this->numDims + this->numCols) * sizeof(GLfloat), (void*)(this->numDims * sizeof(GLfloat)));

    CHECKERRORS();
	this->model = glm::mat4(1.0);
    glUniformMatrix4fv(this->geometryShader->getUniModel(), 1, GL_FALSE, glm::value_ptr(model));
}

void Sphere::Update(UserInputStruct *userInputStruct) {
	glDrawElements(GL_TRIANGLES, sizeof(this->worldArray)/6/sizeof(*this->worldArray), GL_UNSIGNED_INT, 0);
}

void Sphere::Destroy() {
    glDeleteBuffers(1, &this->ebo);
    glDeleteBuffers(1, &this->vbo);
    glDeleteVertexArrays(1, &this->vao);
}

void Sphere::printWorld() {
	std::cout << "The World Coordinates" << std::endl;

	GLfloat *worldPointer = &this->world[0];
	for (int layerIndex = 0; layerIndex <= Sphere::layers; layerIndex++) {
		int layerId = (Sphere::layers/2) - abs(layerIndex - (Sphere::layers/2));
		int noPointsLayeri = noPointsAtLayeri(layerId);

		std::cout << "Layer " << layerIndex << ": ";
		for (int pointIndex = 0; pointIndex < noPointsLayeri; pointIndex++) {
			if (pointIndex != 0) {
				std::cout << ", ";
			}
			std::cout << "(" << *worldPointer << ", " << *(worldPointer + 1) << ", " << *(worldPointer + 2) << ")";
			worldPointer += 3;
		}
		std::cout << std::endl;
	}
}

void Sphere::printWorldIndices() {
	std::cout << "The Polygon Indices" << std::endl;

	GLuint *indicesPointer = &this->indices[0];
	for (int layerIndex = 1; layerIndex <= this->layers; layerIndex++) {
		int correctedLayerIndex = (int)(fabs((float)layerIndex - ((float)(Sphere::layers + 1)/2)) + 0.5);
		int layerNo = 6 * correctedLayerIndex;

		std::cout << "Layer " << layerIndex << ": " << std::endl;
		for (int polygonIndex = 0; polygonIndex < layerNo; polygonIndex++) {
			if (polygonIndex != 0) {
				std::cout << ", ";
			}
			std::cout << "(" << *indicesPointer << ", " << *(indicesPointer + 1) << ", " << *(indicesPointer + 2) << ")";
			indicesPointer += 3;
		}
		std::cout << std::endl;
	}
}

int Sphere::noPointsAtLayeri(int layeri) {
	return (layeri == 0)? 1 : layeri * 6;
}

void Sphere::addCartesian(GLfloat* coordPtr, GLfloat latitude, GLfloat longitude, GLfloat radius) {
	*coordPtr = sin(latitude) * sin(longitude) * radius;
	*++coordPtr = cos(latitude) * radius;
	*++coordPtr = sin(latitude) * cos(longitude) * radius;
}

void Sphere::addIndices(GLuint* indexPtr, GLuint index1, GLuint index2, GLuint index3) {
	*indexPtr = index1;
	*++indexPtr = index2;
	*++indexPtr = index3;
}
