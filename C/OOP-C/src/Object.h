/*
 * Object.h
 *
 *  Created on: Jan 30, 2016
 *      Author: Simon
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include <stdio.h>

#include "DeclareClassMacro.h"

typedef void * (*Constructor) (void *self, va_list *args);
typedef void (*Destructor) (void *self);
typedef int (*Equals) (void *self, void *other);
typedef void (*Print) (void *self, FILE *filePtr);

#define Object_PARENT Root

#define Object_METHODS(PUBLIC, PRIVATE, OVERRIDE, STATIC) \
	_VAR_CAT_WRAP(Object_PARENT,_METHODS)(PUBLIC, PRIVATE, OVERRIDE, STATIC) \
	PRIVATE(Constructor, constructor, objectCtor) \
	PRIVATE(Destructor, destructor, objectDtor) \
	PUBLIC(Equals, equals, objectEquals) \
	PUBLIC(Print, print, objectPrint)

CLASS_INTERFACE(Object)

void* new (Class classInst, ...);
void delete (void *self);

#endif /* OBJECT_H_ */
