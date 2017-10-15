/*
 * Renderer.h
 *
 *  Created on: Dec 29, 2015
 *      Author: Simon
 */

#ifndef RENDERER_H_
#define RENDERER_H_

/*#include "GeometryShader.h"
#include "RenderableObject.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class RenderableObject;

struct point {
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat r;
	GLfloat g;
	GLfloat b;
};

const GLuint numDims = 3;
const GLuint numCols = 3;

struct polygon {
	GLuint p1;
	GLuint p2;
	GLuint p3;
};

class Renderer {

public:
	Renderer();

	int addToRenderList(RenderableObject*);
	void Initialise(GeometryShader*);
	void Render(int, glm::mat4);

	void Destroy();

	~Renderer();

private:
	GLuint vao;
    GLuint vbo;
    GLuint ebo;

	std::vector<RenderableObject*> objectsList;
	std::vector<GLuint> pointOffsetList;
	std::vector<GLuint> polygonOffsetList;

	point *pointArray;
	GLuint pointCounter;

	polygon *polygonArray;
	GLuint polygonCounter;

	GeometryShader *geometryShader;

};*/

#endif /* RENDERER_H_ */
