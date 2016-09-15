/*
 * ShaderManager.h
 *
 *  Created on: Oct 18, 2015
 *      Author: Simon
 */

#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "UtilityManager.h"

#define INACTIVE_SHADER_LOC (-1u)

class ShaderManager : UtilityManager {
public:
	ShaderManager(Config::ConfigHeader configHeader);

	bool Initialise();
	void Destroy();

	static ShaderManager *getActiveShaderManager(void);

	GLuint getShaderProgram();
	GLuint getLayoutLocation(std::string);
	GLuint getUniformLocation(std::string);

	void setUniformValue(std::string shaderLocation, GLfloat uniformValue);

	bool bindTexture(std::string samplerName, GLuint textureID);
	bool bindTexture(std::string samplerName, GLuint textureID, GLenum textureType);
	bool bindArray(std::string arrayName, int numArrays, bool someBool, GLfloat *arrayToSet);

	void useProgram();
	bool validateProgram();

	void enableAttributeLocations();
	void disableAttributeLocations();

	~ShaderManager();

private:
	static const Config::ConfigHeader resourceLoc;
	Config::ConfigHeader header;

	static ShaderManager *currentShaderProgram;

	std::map<std::string, GLenum> samplerMap;

	//std::map<int, std::pair<std::string, GLuint>> layoutList;
	std::vector<GLuint> layoutList;

	GLuint shaderProgram;

	GLuint createShader(GLenum type, Config::ConfigKey key);
	void initialiseTextureLocations(void);
	void parseRawShader(std::string &shaderText);
};

#endif /* SHADERMANAGER_H_ */
