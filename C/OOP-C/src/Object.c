/*
 * Object.c
 *
 *  Created on: Feb 17, 2016
 *      Author: Simon
 */

#define CLASS_DEFINITION

#include <assert.h>
#include <stdlib.h>

#include "_Object.h"

CLASS_PRIVATE_LINKING(Object, Object_METHODS)

// call the supers constructor, maybe need destructors also
/*struct Object * super_new (const void * _class,
	const void * _self, va_list * app) {
	const struct Class * superclass = super(_class);
	assert(superclass->new.method);
	return
	((struct Object * (*) ()) superclass->new.method)
	(_self, app);
}*/

void *new(Class classInst, ...) {
	ObjectDef *_class = classInst;
	ObjectInst *_object;
	va_list ap;

	_object = malloc(_class->size);

	_object->class = _class;

	va_start(ap, classInst);
	_object = _class->constructor(_object, & ap);
	va_end(ap);

	return _object;
}

void delete(void *self) {
	ObjectInst *_object = self;
	ObjectDef *_class = _object->class;

	_class->destructor (self);
	free (self);
}

void *copy(void *self) {
	ObjectInst *_object = self;
	ObjectDef *_class = _object->class;

	return _class->copy(self);
}

int equals(void *self, void *other) {
	ObjectInst *_object = self;
	ObjectDef *_class = _object->class;

	return _class->equals(self, other);
}

void print(void *self, FILE *filePtr) {
	if (!self) {
		return;
	}

	ObjectDef *_class = getClass(self);

	assert(_class->print);

	_class->print(self, filePtr);
}

void * objectCtor(void *self, va_list *args) {
	return self;
}

void objectDtor(void *self) {
	return;
}

void *objectCopy(void *self) {
	return NULL;
}

int objectEquals(void *self, void *other) {
	if (self == other) {
		return 0;
	} else if (self < other) {
		return 1;
	} else {
		return -1;
	}
}

void objectPrint(void *self, FILE *filePtr) {
	ObjectInst *_object = self;
	ObjectDef *_class = _object->class;
	fprintf (filePtr, "%s: %p", _class->name, self);
}
