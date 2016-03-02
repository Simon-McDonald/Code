/*
 * _Object.h
 *
 *  Created on: Feb 17, 2016
 *      Author: Simon
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Object.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define Object_VARIABLES(VARIABLE)
CLASS_PRIVATE_INTERFACE(Object)

int equals (void *self, void *other);
void print (void *self, FILE *filePtr);

void * objectCtor (void *self, va_list *args);
void objectDtor (void *self);
int objectEquals (void *self, void *other);
void objectPrint (void *self, FILE *filePtr);

/*struct _ObjectInst {
	Class *class;
};

extern const Class _ObjectClass;*/

#endif /* _OBJECT_H_ */
