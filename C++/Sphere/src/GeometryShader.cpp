/*
 * GeometryShader.cpp
 *
 *  Created on: Oct 13, 2015
 *      Author: Simon
 */

#include "GeometryShader.h"
#include <iostream>
#include <cstdlib>

const GLchar* vertexShaderSrc = GLSL(
    in vec3 position;
    in vec3 color;
    out vec3 Color;

    uniform mat4 view;
    uniform mat4 proj;
    uniform mat4 model;

    void main() {
       Color = color;
       gl_Position = proj * view * model * vec4(position, 1.0);
    }
);

const GLchar* fragmentShaderSrc = GLSL(
    in vec3 Color;
    out vec4 outColor;
    void main() {
       outColor = vec4(Color, 1.0);
    }
);

bool GeometryShader::Initialise() {
	//////////////////////////////////////////
	// Create and compile the vertex shader //
	//////////////////////////////////////////
	this->vertexShader = this->createShader(GL_VERTEX_SHADER, vertexShaderSrc);
	if (this->vertexShader == -1u) {
		exit (1);
	}
	////////////////////////////////////////////
	// Create and compile the fragment shader //
	////////////////////////////////////////////
	this->fragmentShader = this->createShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
	if (this->fragmentShader == -1u) {
		exit (1);
	}
	///////////////////////////////////////////////////////////////
	// Link the vertex and fragment shader into a shader program //
	///////////////////////////////////////////////////////////////
	// compiling the program shaders and connecting them together
	this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, this->vertexShader);
    glAttachShader(this->shaderProgram, this->fragmentShader);
    glBindFragDataLocation(this->shaderProgram, 0, "outColor");
    glLinkProgram(this->shaderProgram);
    glUseProgram(this->shaderProgram);

    this->uniModel = glGetUniformLocation(this->shaderProgram, "model");

    return true;
}

GLuint GeometryShader::getShaderProgram() {
	return this->shaderProgram;
}

GLuint GeometryShader::getUniModel() {
	return this->uniModel;
}

void GeometryShader::Destroy() {
    glDeleteProgram(this->shaderProgram);
    glDeleteShader(this->fragmentShader);
    glDeleteShader(this->vertexShader);
}

GLuint GeometryShader::createShader(GLenum type, const GLchar* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		std::cout << "Shader of type " << type << " failed to compile!";
		char buffer[512];
		glGetShaderInfoLog(shader, 512, NULL, buffer);

		for (int index = 0; index < 512; index++) {
			std::cout << buffer[index];
		}
		std::cout << std::endl;

		return -1;
	} else {
		std::cout << "Shader of type " << type << " compiled successfully!" << std::endl;
	}

    return shader;
}

