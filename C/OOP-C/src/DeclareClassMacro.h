/*
 * DeclareClassMacro.h
 *
 *  Created on: Feb 23, 2016
 *      Author: Simon
 */

#ifndef DECLARECLASSMACRO_H_
#define DECLARECLASSMACRO_H_

typedef const void *Class;

struct _RootInst {
	//Class *const class;
};
typedef struct _RootInst RootInst;

#define _VAR_CAT(start,end) start##end
#define _VAR_CAT_WRAP(start,end) _VAR_CAT(start,end)
#define _VAR_CAT2(start,end) start##end

#define Root_METHODS(PUBLIC, PRIVATE, OVERRIDE, STATIC)

/*
struct _ShapeInst;
typedef struct _ShapeInst ShapeInst;

extern const Class *const ShapeClass;

struct _ShapeRef {
	void (*SetPosition) (void *self, Vector2D newPosition);
	void (*Move) (void *self, Vector2D translation);
};
typedef struct _ShapeRef ShapeRef;
extern const ShapeRef *const Shape;
 */
#define _INTERFACE_PUBLIC(method_type, method_name, method_instance) method_type method_name;
#define _INTERFACE_PRIVATE(method_type, method_name, method_instance)
#define _INTERFACE_OVERRIDE(method_name, method_instance)
#define _INTERFACE_STATIC(method_type, method_name, method_instance) method_type method_name;

#define CLASS_INTERFACE(class) \
	extern Class const class##Class; \
	 \
	struct _##class##Inst; \
	typedef struct _##class##Inst class##Inst; \
	 \
	struct _##class##Ref { \
		_VAR_CAT(class,_METHODS)(_INTERFACE_PUBLIC, _INTERFACE_PRIVATE, _INTERFACE_OVERRIDE, _INTERFACE_STATIC) \
	}; \
	typedef const struct _##class##Ref class##Ref; \
	extern class##Ref class;

/*
struct _ShapeInst {
	ObjectInst _;
	Vector2D centreCoord;
};
extern const ShapeDef _ShapeClass;
 */
#define CLASS_DEFINE_VARIABLES(type, name) type name;

#define CLASS_PRIVATE_INTERFACE(classType) \
	struct _##classType##Inst { \
		Class class; \
		_VAR_CAT_WRAP(classType##_PARENT,Inst) _; \
		_VAR_CAT(classType,_VARIABLES)(CLASS_DEFINE_VARIABLES) \
	}; \
	 \
	struct _##classType##Def; \
	typedef const struct _##classType##Def classType##Def; \
	extern classType##Def _##classType##Class;

/*
ShapeRef Shape = {
	.SetPosition = setPosition,
	.Move = move
};

struct _ShapeDef {
	char *name;
	Class *const super;
	size_t size;
	void * (*ctor) (void *self, va_list *args);
	void (*dtor) (void *self);
	int (*equals) (void *self, void *other);
	void (*print) (void *self, FILE *filePtr);
	void (*setPosition) (void *self, Vector2D newPosition);
	void (*move) (void *self, Vector2D translation);
};
typedef struct _ShapeDef ShapeDef;

const ShapeDef _ShapeClass = {
	.name = "Shape",
	.super = &_ObjectClass,
	.size = sizeof (ShapeInst),
	.ctor = shapeCtor,
	.dtor = shapeDtor,
	.equals = NULL,
	.print = shapePrint,
	.setPosition = NULL,
	.move = NULL
};
const Class *ShapeClass = &_ShapeClass;
 */
/*  .##method_name = method_name, */

#define _LINKING_PUBLIC(method_type, method_name, method_instance) method_name,
#define _LINKING_PRIVATE(method_type, method_name, method_instance)
#define _LINKING_OVERRIDE(method_name, method_instance)
#define _LINKING_STATIC(method_type, method_name, method_instance) method_name,

#define _LINKING_VARIABLES_DEF(x)

#define _LINKING_DECLARE_PUBLIC(x,y,z) x y;
#define _LINKING_DECLARE_PRIVATE(x,y,z) x y;
#define _LINKING_DECLARE_OVERRIDE(x,y)
#define _LINKING_DECLARE_STATIC(x,y,z)
#define _LINKING_DEFINE_PUBLIC(x,y,z) . y = z,
#define _LINKING_DEFINE_PRIVATE(x,y,z) . y = z,
#define _LINKING_DEFINE_OVERRIDE(y,z) . y = z,
#define _LINKING_DEFINE_STATIC(x,y,z)

/*#define _DEFINE_CLASS(class) \
	variables(...)*/

/* _VAR_CAT_WRAP(_VAR_CAT2(&_,class##_PARENT),Class,) */

/* _VAR_CAT(class, _VARIABLES)(_LINKING_VARIABLES_DEF) \ */

#define CLASS_PRIVATE_LINKING(class) \
	const class##Ref class = { \
		_VAR_CAT(class,_METHODS)(_LINKING_PUBLIC, _LINKING_PRIVATE, _LINKING_OVERRIDE, _LINKING_STATIC) \
	}; \
	 \
	struct _##class##Def { \
		char *name; \
		Class super; \
		size_t size; \
		_VAR_CAT(class,_METHODS)(_LINKING_DECLARE_PUBLIC, _LINKING_DECLARE_PRIVATE, _LINKING_DECLARE_OVERRIDE, _LINKING_DECLARE_STATIC) \
	}; \
	 \
	const class##Def _##class##Class = { \
		#class, \
		NULL, \
		sizeof (class##Inst), \
		_VAR_CAT(class,_METHODS)(_LINKING_DEFINE_PUBLIC, _LINKING_DEFINE_PRIVATE, _LINKING_DEFINE_OVERRIDE, _LINKING_DEFINE_STATIC) \
	}; \
	Class const class##Class = &_##class##Class;
	/*	parent(...) \ 	methods(...) \*/
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////








/*struct _Vector2D {
	float x;
	float y;
};
typedef struct _Vector2D Vector2D;







typedef void (*moveType) (void*, Vector2D);
typedef void (*getCoordType) (void*);


#define pointDefinition(NEW, OVERRIDE, STATIC) \
	NEW()*/





#endif /* DECLARECLASSMACRO_H_ */
