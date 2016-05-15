/*
 * Triangle.c
 *
 *  Created on: Apr 18, 2016
 *      Author: Simon
 */

#include "_Triangle.h"

CLASS_PRIVATE_LINKING(Triangle, Triangle_METHODS)

void trianglePrint (void *self, FILE *filePtr) {
	//TriangleInst *_object = self;
	fprintf (filePtr, "This is a triangle!!!!");
}


