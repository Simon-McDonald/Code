/*
 * Object.c
 *
 *  Created on: Feb 17, 2016
 *      Author: Simon
 */

#define CLASS_DEFINITION

#include <stdlib.h>

#include "_Object.h"

CLASS_PRIVATE_LINKING(Object)

int equals (void *self, void *other) {
	ObjectInst *_object = self;
	ObjectDef *_class = _object->class;

	return _class->equals (self, other);
}

void print (void *self, FILE *filePtr) {
	ObjectInst *_object = self;
	ObjectDef *_class = _object->class;

	_class->print (self, filePtr);
}



/*int equals (void *self, void *other);
void print (void *self, FILE *filePtr);

ObjectRef Object = {
	.Equals = equals,
	.Print = print
};

void * objectCtor (void *self, va_list *args);
void objectDtor (void *self);
int objectEquals (void *self, void *other);
void objectPrint (void *self, FILE *filePtr);

struct _ObjectDef {
	char *name;
	Class *const super;
	size_t size;
	void * (*ctor) (void *self, va_list *args);
	void (*dtor) (void *self);
	int (*equals) (void *self, void *other);
	void (*print) (void *self, FILE *filePtr);
};
typedef struct _ObjectDef ObjectDef;

const ObjectDef _ObjectClass = {
	.name = "Object",
	.super = NULL,
	.size = sizeof (ObjectInst),
	.ctor = objectCtor,
	.dtor = objectDtor,
	.equals = objectEquals,
	.print = objectPrint
};
const Class *ObjectClass = &_ObjectClass;*/

void * new (Class classInst, ...) {
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

void delete (void *self) {
	ObjectInst *_object = self;
	ObjectDef *_class = _object->class;

	_class->destructor (self);
	free (self);
}

/*int equals (void *self, void *other) {
	ObjectInst *_object = self;
	Class *_class = _object->class;

	return _class->equals (self, other);
}

void print (void *self, FILE *filePtr) {
	ObjectInst *_object = self;
	Class *_class = _object->class;

	_class->print (self, filePtr);
}*/

void * objectCtor (void *self, va_list *args) {
	return self;
}

void objectDtor (void *self) {
	return;
}

int objectEquals (void *self, void *other) {
	if (self == other) {
		return 0;
	} else if (self < other) {
		return 1;
	} else {
		return -1;
	}
}

void objectPrint (void *self, FILE *filePtr) {
	ObjectInst *_object = self;
	ObjectDef *_class = _object->class;
	fprintf (filePtr, "%s: %p", _class->name, self);
}

/*struct Object * super_new (const void * _class,
	const void * _self, va_list * app) {
	const struct Class * superclass = super(_class);
	assert(superclass->new.method);
	return
	((struct Object * (*) ()) superclass->new.method)
	(_self, app);
}*/
