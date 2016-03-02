/*
 * Shape.c
 *
 *  Created on: Feb 21, 2016
 *      Author: Simon
 */

#include "_Shape.h"

CLASS_PRIVATE_LINKING(Shape)

void translate (void *self, Vector2D translation) {
	ShapeInst *_object = self;
	ShapeDef *_class = _object->class;

	return _class->translate (self, translation);
}

void setLocation (void *self, Vector2D location) {
	ShapeInst *_object = self;
	ShapeDef *_class = _object->class;

	return _class->setLocation (self, location);
}

void * shapeCtor (void *self, va_list *args) {
	// probably should call supers constructor

	ShapeInst *_shape = self;

	Vector2D centre = va_arg (*args, Vector2D);

	printf ("x: %f, y: %f\n", centre.x, centre.y);

	_shape->centreLoc = centre;

	return self;
}

void shapeDtor (void *self) {
	// no cleanup needs to be done

	// probably should call supers distructor

	return;
}

void shapePrint (void *self, FILE *filePtr) {
	ShapeInst *_object = self;
	fprintf (filePtr, "Shape: Centre: (%f,%f)", _object->centreLoc.x, _object->centreLoc.y);
}

void shapeSetLocation (void *self, Vector2D newPosition) {
	ShapeInst *_object = self;
	_object->centreLoc = newPosition;
}

void shapeTranslate (void *self, Vector2D translation) {
	ShapeInst *_object = self;
	Vector2D temp = {_object->centreLoc.x + translation.x,
					 _object->centreLoc.y + translation.y};
	_object->centreLoc = temp;
}


/*
// Selector Functions!
void setPosition (void *self, Vector2D newPosition);
void move (void *self, Vector2D translation);

// Implementations
void * shapeCtor (void *self, va_list *args);
void shapeDtor (void *self);
void shapePrint (void *self, FILE *filePtr);


ShapeRef Shape = {
	.SetPosition = setPosition,
	.Move = move
};

struct _ShapeDef {
	char *name;
	Class *const super;
	size_t size;
	void * (*ctor) (void *self, va_list *args);
	void (*dtor) (void *self);
	int (*equals) (void *self, void *other);
	void (*print) (void *self, FILE *filePtr);
	void (*setPosition) (void *self, Vector2D newPosition);
	void (*move) (void *self, Vector2D translation);
};
typedef struct _ShapeDef ShapeDef;

const ShapeDef _ShapeClass = {
	.name = "Shape",
	.super = &_ObjectClass,
	.size = sizeof (ShapeInst),
	.ctor = shapeCtor,
	.dtor = shapeDtor,
	.equals = NULL,
	.print = shapePrint,
	.setPosition = NULL,
	.move = NULL
};
const Class *ShapeClass = &_ShapeClass;
*/
