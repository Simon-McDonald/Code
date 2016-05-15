/*
 * _Object.h
 *
 *  Created on: Feb 17, 2016
 *      Author: Simon
 */

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Object.h"

#include <stdlib.h>
#include <stdarg.h>

#define Object_VARIABLES(VARIABLE) // contains no new variables

CLASS_PRIVATE_INTERFACE(Object, Root)

// helper macro, MUST be after CLASS_PRIVATE_INTERFACE
#define getClass(classInst) ((ObjectInst *) classInst)->class

void *copy(void *self);
int equals(void *self, void *other);
void print(void *self, FILE *filePtr);

void *objectCtor(void *self, va_list *args);
void objectDtor(void *self);
void *objectCopy(void *self);
int objectEquals(void *self, void *other);
void objectPrint(void *self, FILE *filePtr);

#endif /* _OBJECT_H_ */
