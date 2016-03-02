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

typedef void (*Translate) (void *self, Vector2D translation);
typedef void (*SetLocation) (void *self, Vector2D location);

#define Shape_PARENT Object

#define Shape_METHODS(PUBLIC, PRIVATE, OVERRIDE, STATIC) \
	_VAR_CAT_WRAP(Shape_PARENT,_METHODS)(PUBLIC, PRIVATE, OVERRIDE, STATIC) \
	OVERRIDE(constructor, shapeCtor) \
	OVERRIDE(destructor, shapeDtor) \
	OVERRIDE(print, shapePrint) \
	PUBLIC(Translate, translate, shapeTranslate) \
	PUBLIC(SetLocation, setLocation, shapeSetLocation)

CLASS_INTERFACE(Shape)


/*struct _ShapeInst;
typedef struct _ShapeInst ShapeInst;

extern const Class *const ShapeClass;

struct _ShapeRef {
	void (*SetPosition) (void *self, Vector2D newPosition);
	void (*Move) (void *self, Vector2D translation);
};
typedef struct _ShapeRef ShapeRef;
extern const ShapeRef *const Shape;*/

#endif /* SHAPE_H_ */
