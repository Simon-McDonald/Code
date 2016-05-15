/*
 * Shape.h
 *
 *  Created on: Feb 21, 2016
 *      Author: Simon
 */

#ifndef SHAPE_H_
#define SHAPE_H_

#include "Object.h"

struct _Vector2D {
	float x;
	float y;
};
typedef struct _Vector2D Vector2D;

typedef void (*setVector2D)(void *self, Vector2D translation);
typedef Vector2D (*getVector2D)(void *self);
typedef float (*getFloat)(void *self);

#define Shape_PARENT Object

#define Shape_METHODS(PUBLIC, PRIVATE, OVERRIDE, STATIC) \
	parent_METHODS(Shape, PUBLIC, PRIVATE, OVERRIDE, STATIC) \
	/*_VAR_CAT_WRAP(Shape_PARENT,_METHODS)(PUBLIC, PRIVATE, OVERRIDE, STATIC)*/ \
	OVERRIDE(constructor, shapeCtor) \
	OVERRIDE(destructor, shapeDtor) \
	OVERRIDE(print, shapePrint) \
	PUBLIC(setVector2D, translate, shapeTranslate) \
	PUBLIC(setVector2D, setLocation, shapeSetLocation)

CLASS_INTERFACE(Shape)

#endif /* SHAPE_H_ */
