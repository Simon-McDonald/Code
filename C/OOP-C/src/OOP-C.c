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
#include "Triangle.h"

/*
Inheritance Tree:

       Root
        |
      Object
        |
      Shape
     /     \
 Square   Triangle

*/

int main ()
{
	//printf ("The result: %s\n", cat(test,2));








	ObjectInst *object = new (ObjectClass);

	TriangleInst *triangle = new (TriangleClass);

	Vector2D centre = {5, 6};
	ObjectInst *shape = new (ShapeClass, centre);

	Object.print(object, stdout);

	printf ("\nTest %i\n", Object.equals (object, object));

	Shape.print(shape, stdout);

	Triangle.print(triangle, stdout);

	return 0;
}
