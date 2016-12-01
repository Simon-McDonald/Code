/*
 * ErrorCheck.h
 *
 *  Created on: Nov 28, 2015
 *      Author: Simon
 */

#ifndef CHECKERRORS

#include <iostream>

#define CHECKERRORS() \
do { \
	GLenum err (glGetError()); \
	while (err != GL_NO_ERROR) { \
		std::string error; \
		switch (err) { \
			case GL_INVALID_OPERATION: error = "GL_INVALID_OPERATION"; break; \
			case GL_INVALID_ENUM: error = "GL_INVALID_ENUM"; break; \
			case GL_INVALID_VALUE: error = "GL_INVALID_VALUE"; break; \
			case GL_OUT_OF_MEMORY: error = "GL_OUT_OF_MEMORY"; break; \
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "GL_INVALID_FRAMEBUFFER_OPERATION"; break; \
		} \
		std::cout << "OpenGL ERROR " << err << ": " << error << " - " << __FILE__ << ":" << __LINE__ << std::endl; \
		err = glGetError(); \
	} \
} while (0)


#endif /* CHECKERRORS */
