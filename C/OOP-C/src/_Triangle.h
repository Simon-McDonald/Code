/*
 * _Triangle.h
 *
 *  Created on: Apr 18, 2016
 *      Author: Simon
 */

#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "_Shape.h"
#include "Triangle.h"

#define Triangle_VARIABLES(VARIABLE)

CLASS_PRIVATE_INTERFACE(Triangle, Shape)

void trianglePrint (void *self, FILE *filePtr);

#endif /* _TRIANGLE_H_ */
