/*
 * Sphere.h
 *
 *  Created on: Oct 7, 2015
 *      Author: Simon
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include "OpenGLController.h"
#include "GeometryShader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Sphere {

public:
	void Initialise(GeometryShader*);
	void Update(UserInputStruct*);
	void Destroy();
	void addCartesian(GLfloat*, GLfloat, GLfloat, GLfloat);
	void addIndices(GLuint*, GLuint, GLuint, GLuint);

	void printWorld();
	void printWorldIndices();

	// Must be EVEN and >= 2
	static const int layers = 2;
	static const int numberPoints = 1.5 * layers * layers + 2;
	static const int numberPolygons = 3 * layers * layers;

	static const int numDims = 3;
	static const int numCols = 3;

	GLfloat world[numberPoints * 3];
	GLuint indices[numberPolygons * 3];
	GLfloat worldArray[numberPolygons * 3 * 6];

private:

	GLuint vao;
    GLuint vbo;
    GLuint ebo;

    GeometryShader *geometryShader = NULL;

    //GLint uniModel;
    glm::mat4 model;

    int noPointsAtLayeri(int);


	//Coordinate pointList[numberPoints];
	//int pointLayerNumber[layers + 1];
	//Coordinate *pointLayerArray[layers + 1];

};

#endif /* SPHERE_H_ */
