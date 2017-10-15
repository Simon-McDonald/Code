/*
 * RenderableObject.h
 *
 *  Created on: Dec 29, 2015
 *      Author: Simon
 */

#ifndef RENDERABLEOBJECT_H_
#define RENDERABLEOBJECT_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "WorldObject.h"

class Renderer;

class RenderableObject : public WorldObject {

public:
	RenderableObject(int, WorldPosition, float, Renderer*, int);

	virtual int getNumberPoints();
	virtual int getNumberPolygons();
	virtual void populateGeometry(struct point*, struct polygon*);

	float getObjectRadius();

	virtual void update();
	void display();

	virtual ~RenderableObject();

	bool checkIfAlive();
	void kill();

protected:
	int layers;
    Renderer *renderer;
    GLuint renderId;

    int colour;

	glm::mat4 model;

	int numberPoints;
	int numberPolygons;

	float scale;
	bool isAlive;

    int noPointsAtLayeri(int);
    void addCartesian(point*, GLfloat, GLfloat, GLfloat);

};

#endif /* RENDERABLEOBJECT_H_ */
