/*
 * _Shape.h
 *
 *  Created on: Feb 21, 2016
 *      Author: Simon
 */

#ifndef _SHAPE_H_
#define _SHAPE_H_

#include "_Object.h"
#include "Shape.h"

#define Shape_VARIABLES(VARIABLE) \
	VARIABLE(Vector2D, centreLoc)

CLASS_PRIVATE_INTERFACE(Shape)

void translate (void *self, Vector2D translation);
void setLocation (void *self, Vector2D location);

void * shapeCtor (void *self, va_list *args);
void shapeDtor (void *self);

void shapePrint (void *self, FILE *filePtr);

void shapeTranslate (void *self, Vector2D translation);
void shapeSetLocation (void *self, Vector2D location);

/*struct _ShapeInst {
	ObjectInst _;
	Vector2D centreCoord;
};*/

#endif /* _SHAPE_H_ */
