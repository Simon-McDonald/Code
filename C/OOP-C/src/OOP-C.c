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

/*#define cat(start, end) start ## end

#define test1 "aaa"
#define test2 cat(test, 1)*/

/*#define a_PARENT
#define b_PARENT a
#define c_PARENT b
#define d_PARENT c

#define a_STATE printf("a\n");
#define b_STATE printf("b\n");
#define c_STATE printf("c\n");
#define d_STATE printf("d\n");

#define _VAR_CAT2(start,end) start##end
#define _VAR_CAT_WRAP2(start,end) _VAR_CAT(start,end)

#define display(letter) \
		display2(letter) \
		_VAR_CAT2(letter,_STATE)

#define display2(letter) \
		display(_VAR_CAT2(letter,_PARENT)) \
		_VAR_CAT2(letter,_STATE)*/

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
