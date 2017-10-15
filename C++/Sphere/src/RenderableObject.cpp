/*
 * RenderableObject.cpp
 *
 *  Created on: Dec 29, 2015
 *      Author: Simon
 */

#include "RenderableObject.h"

#include "ErrorCheck.h"

RenderableObject::RenderableObject(int numLayers, WorldPosition position, float scale, Renderer *renderer, int colour) : WorldObject(position), layers(numLayers), renderer(renderer), colour(colour), scale(scale), isAlive(true) {
	assert(numLayers % 2 == 0);

	this->numberPoints = 1.5 * this->layers * this->layers + 2;
	this->numberPolygons = 3 * this->layers * this->layers;
	this->model = glm::mat4(scale);

	/*float x = sin(position.getLatitude()) * sin(position.getLongitude()) * position.getRadius();
	float y = cos(position.getLatitude()) * position.getRadius();
	float z = sin(position.getLatitude()) * cos(position.getLongitude()) * position.getRadius();*/

	/*this->model[3][0] = x;
	this->model[3][1] = y;
	this->model[3][2] = z;
	this->model[3][3] = 1;*/

	/*std::cout << this->model[0][0] << ", " << this->model[0][1] << ", " << this->model[0][2] << ", " << this->model[0][3] << std::endl;
	std::cout << this->model[1][0] << ", " << this->model[1][1] << ", " << this->model[1][2] << ", " << this->model[1][3] << std::endl;
	std::cout << this->model[2][0] << ", " << this->model[2][1] << ", " << this->model[2][2] << ", " << this->model[2][3] << std::endl;
	std::cout << this->model[3][0] << ", " << this->model[3][1] << ", " << this->model[3][2] << ", " << this->model[3][3] << std::endl;*/

	this->renderId = renderer->addToRenderList(this);
}

void RenderableObject::populateGeometry(point *pointArray, polygon *polygonArray) {

	point *pointArrayPtr = pointArray;
	GLfloat radius = 0.5f;

	// generate the triplet points for the vertices of the sphere
	for (int layerIndex = 0; layerIndex <= this->layers; layerIndex++) {
		int layerId = (this->layers/2) - abs(layerIndex - (this->layers/2));
		int noPointsLayeri = noPointsAtLayeri(layerId);

		for (int layerPointIndex = 0; layerPointIndex < noPointsLayeri; layerPointIndex++) {
			GLfloat latitude = 180.0f / GLfloat(this->layers) * GLfloat(layerIndex);
			GLfloat longitude = 360.0f / noPointsLayeri * layerPointIndex;
			this->addCartesian(pointArrayPtr, latitude * M_PI / 180, longitude * M_PI / 180, radius);

			int tempIndex = rand()*3/RAND_MAX;

			pointArrayPtr->r = 0.0;
			pointArrayPtr->g = 0.0;
			pointArrayPtr->b = 0.0;

			switch (this->colour) {
			case 0:
				switch (tempIndex) {
				case 0:
					pointArrayPtr->r = 1.0;
					break;
				case 1:
					pointArrayPtr->g = 1.0;
					break;
				case 2:
					pointArrayPtr->b = 1.0;
					break;
				}
				break;
			case 1:
				pointArrayPtr->r = 1.0;
				break;
			case 2:

				break;
			}

			pointArrayPtr++;
		}
	}

	CHECKERRORS();

	polygon *polygonArrayPtr = polygonArray;

	for (GLuint layerIndex = 1; layerIndex <= (GLuint)this->layers; layerIndex++) {
		GLuint correctedLayerIndex = ((this->layers + 1)/2) - abs(layerIndex - ((this->layers + 1)/2));
		GLuint correctedPrevLayerIndex = ((this->layers + 1)/2) - abs((layerIndex - 1) - ((this->layers + 1)/2));

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

					polygonArrayPtr->p1 = upperFaceIndex;
					polygonArrayPtr->p2 = lowerFaceIndex;
					polygonArrayPtr->p3 = incLowerIndex;

					//this->addIndices(triangleIndex, upperFaceIndex, lowerFaceIndex, incLowerIndex);
					lowerFaceIndex++;
				} else {
					GLuint incUpperIndex = (upperFaceIndex + 1 == upperStartIndex + upperLayerNo) ? upperStartIndex : upperFaceIndex + 1;

					polygonArrayPtr->p1 = upperFaceIndex;
					polygonArrayPtr->p2 = incUpperIndex;
					polygonArrayPtr->p3 = lowerFaceIndex;

					//this->addIndices(triangleIndex, upperFaceIndex, incUpperIndex, lowerFaceIndex);
					upperFaceIndex++;
				}
				startBottom = !startBottom;

				polygonArrayPtr++;
				//triangleIndex += 3;
			}
		}
	}

	CHECKERRORS();
}

float RenderableObject::getObjectRadius() {
	return 0.5 * this->scale;
}

int RenderableObject::getNumberPoints() {
	return this->numberPoints;
}

int RenderableObject::getNumberPolygons() {
	return this->numberPolygons;
}

bool RenderableObject::checkIfAlive() {
	return this->isAlive;
}

void RenderableObject::kill() {
	this->isAlive = false;
}

void RenderableObject::update() {

}

void RenderableObject::display() {
	if (!this->isAlive) {
		return;
	}

	this->model[3] = this->entityPos.getCartesianCoords();
	this->renderer->Render(this->renderId, this->model);
}

void RenderableObject::addCartesian(point *coordPtr, GLfloat latitude, GLfloat longitude, GLfloat radius) {
	coordPtr->x = sin(latitude) * sin(longitude) * radius;
	coordPtr->y = cos(latitude) * radius;
	coordPtr->z = sin(latitude) * cos(longitude) * radius;
}

int RenderableObject::noPointsAtLayeri(int layeri) {
	return (layeri == 0)? 1 : layeri * 6;
}

RenderableObject::~RenderableObject() {}
