/*
 * Example.h
 *
 *  Created on: May 14, 2016
 *      Author: Simon
 */

#ifndef EXAMPLE_H_
#define EXAMPLE_H_
/*
// Include the public header file of the parent class
#include "Object.h"

// Any appropriate macros, constants and typedefs for the class module.

// typedefs for all public selector functions to be created by the class. These
// will be the templates of all function calls on each class instance.
typedef int (*getInt)(void *self);
typedef void (*setInt)(void *self, int value);

// Define the parent of the newly defined class.
#define Example_PARENT Object

// Define the classes methods. Parent methods MUST be included first. The
// 'parent_METHODS' macro expands using Example_PARENT to ultimately call
// 'Object_METHODS(...)'.
#define Example_METHODS(PUBLIC, PRIVATE, OVERRIDE, STATIC) \
	parent_METHODS(Example, PUBLIC, PRIVATE, OVERRIDE, STATIC) \
	OVERRIDE(construcotr, exampleCtor) \
	OVERRIDE(destructor, exampleCtor) \
	OVERRIDE(print, examplePrint) \
	OVERRIDE(equals, example) \
	PUBLIC(setInt, setNumber, exampleSetNumber) \
	PUBLIC(getInt, getNumber, exampleGetNumber)

// Generate the class interface. The type ExampleClass is used with the new
// function to create instances of the Example class. ExampleInst can be used
// as a pointer to instances of the class. Finally the instance Example of
// ExampleRef points to the public methods of the Example class, inherited,
// static and new.
CLASS_INTERFACE(Example)
*/
#endif /* EXAMPLE_H_ */
