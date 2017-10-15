/*
 * GeometryShader.h
 *
 *  Created on: Oct 18, 2015
 *      Author: Simon
 */

#ifndef GEOMETRYSHADER_H_
#define GEOMETRYSHADER_H_

#define GLSL(src) "#version 150 core\n" #src

#include "OpenGLController.h"

#include <iostream>
#include <chrono>
#include <stdio.h>
#include <cmath>

// Vertex shader
extern const char* vertexShaderSrc;
extern const char* fragmentShaderSrc;

class GeometryShader {
public:
	bool Initialise();
	void Destroy();

	GLuint getShaderProgram();
	GLuint getUniModel();

private:
	GLuint createShader(GLenum, const GLchar*);
	GLuint shaderProgram;

	GLuint vertexShader;
	GLuint fragmentShader;

    GLint uniModel;

};

#endif /* GEOMETRYSHADER_H_ */
