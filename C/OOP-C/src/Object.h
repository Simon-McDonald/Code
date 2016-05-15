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

/*
 * Define the class method prototypes.
 */
typedef void *(*Constructor)(void *self, va_list *args);
typedef void (*Destructor)(void *self);
typedef void *(*Copy)(void *self);
typedef int (*Equals)(void *self, void *other);
typedef void (*Print)(void *self, FILE *filePtr);

#define Object_PARENT Root

// Recall, this is the only METHODS macro that does not include the super.
#define Object_METHODS(PUBLIC, PRIVATE, OVERRIDE, STATIC) \
	PRIVATE(Constructor, constructor, objectCtor) \
	PRIVATE(Destructor, destructor, objectDtor) \
	PUBLIC(Copy, copy, objectCopy) \
	PUBLIC(Equals, equals, objectEquals) \
	PUBLIC(Print, print, objectPrint)

CLASS_INTERFACE(Object)

// Special selectors, just so they are not connected to a particular class Ref
void* new (Class classInst, ...);
void delete (void *self);

#endif /* OBJECT_H_ */
