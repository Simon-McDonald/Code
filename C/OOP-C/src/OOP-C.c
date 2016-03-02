/*
 ============================================================================
 Name        : OOP-C.c
 Author      : Simon
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>

#include "Object.h"
#include "Shape.h"

int main ()
{
	ObjectInst *object = new (ObjectClass);

	Vector2D centre = {5, 6};
	ObjectInst *shape = new (ShapeClass, centre);

	Object.print(object, stdout);

	printf ("\nTest %i\n", Object.equals (object, object));

	Shape.print(shape, stdout);

	return 0;
}
