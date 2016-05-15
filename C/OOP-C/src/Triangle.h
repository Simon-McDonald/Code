/*
 * Triangle.h
 *
 *  Created on: Apr 18, 2016
 *      Author: Simon
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Shape.h"

#define Triangle_PARENT Shape

#define Triangle_METHODS(PUBLIC, PRIVATE, OVERRIDE, STATIC) \
	Shape_METHODS(PUBLIC, PRIVATE, OVERRIDE, STATIC) \
	OVERRIDE(print, trianglePrint)

CLASS_INTERFACE(Triangle)

#endif /* TRIANGLE_H_ */
