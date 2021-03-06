/*
 * ShaderManager.cpp
 *
 *  Created on: Oct 13, 2015
 *      Author: Simon
 */

#include <iostream>
#include <fstream>
#include <string>

#include "CheckErrors.h"
#include "ShaderManager.h"

const Config::ConfigHeader ShaderManager::resourceLoc = "RESOURCES";
ShaderManager* ShaderManager::currentShaderProgram = nullptr;

ShaderManager *ShaderManager::getActiveShaderManager(void) {
	return ShaderManager::currentShaderProgram;
} /* ShaderManager::getActiveShaderManager */

ShaderManager::ShaderManager(Config::ConfigHeader configHeader) :
	header(configHeader), shaderProgram(-1u) {
} /* ShaderManager::ShaderManager */

bool ShaderManager::Initialise(void) {
	GLuint vertexShader = this->createShader(GL_VERTEX_SHADER, "vertex_shader");
	GLuint fragmentShader = this->createShader(GL_FRAGMENT_SHADER, "fragment_shader");
	GLuint geometryShader = this->createShader(GL_GEOMETRY_SHADER, "geometry_shader");

	this->shaderProgram = glCreateProgram();

    glAttachShader(this->shaderProgram, vertexShader);
    glAttachShader(this->shaderProgram, fragmentShader);
    glAttachShader(this->shaderProgram, geometryShader);

    glLinkProgram(this->shaderProgram);

    GLint linkStatus;
    glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &linkStatus);

	CHECKERRORS();

    if (linkStatus != GL_TRUE) {
		const int bfrLen = 512;
		char errorMsgBfr[bfrLen];
		glGetProgramInfoLog(this->shaderProgram, bfrLen, NULL, errorMsgBfr);

    	this->getLogger().Log(Logger::error, LOG_ARGS, "Failed to link shader program '%s', error: %s", this->header.c_str(), errorMsgBfr);

    	return false;
    } else {
    	this->getLogger().Log(Logger::info, LOG_ARGS, "Linked shader program '%s'", this->header.c_str());
    }

	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	CHECKERRORS();

	this->useProgram();

	this->initialiseTextureLocations();

    return true;
} /* ShaderManager::Initialise */

GLuint ShaderManager::createShader(GLenum type, Config::ConfigKey key) {
	if (!this->getConfig().containsHeader(ShaderManager::resourceLoc) ||
	    !this->getConfig().containsKey(ShaderManager::resourceLoc, "shader_dir")) {
		this->getLogger().Log(Logger::info, LOG_ARGS, "No shader folder path");
		return -1u;
	}
	std::string path = this->getConfig().getString(ShaderManager::resourceLoc, "shader_dir");

	if (!this->getConfig().containsHeader(this->header) ||
	    !this->getConfig().containsKey(this->header, key)) {
		return -1u;
	}
	std::string value = this->getConfig().getString(this->header, key);

	std::string shaderText;
	std::string shaderFile = path + "/" + value;
	std::ifstream shaderStream(shaderFile.c_str(), std::ios::in);

	if (shaderStream.is_open()) {
		std::string Line = "";
		while (getline(shaderStream, Line)) {
			shaderText += "\n" + Line;
		}
		shaderStream.close();
	} else {
		this->getLogger().Log(Logger::error, LOG_ARGS,
			"Cannot open shader %s!", shaderFile.c_str());
		return -1u;
	}

    GLuint shader = glCreateShader(type);
    char const * shaderSrcPtr = shaderText.c_str();
    glShaderSource(shader, 1, &shaderSrcPtr, nullptr);
    glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		const int bfrLen = 512;
		char errorMsgBfr[bfrLen];
		glGetShaderInfoLog(shader, bfrLen, NULL, errorMsgBfr);

		std::cout << "Error in file: " << shaderFile << std::endl;
		std::cout << errorMsgBfr << std::endl;
		//this->getLogger().Log(Logger::error, LOG_ARGS, "Shader '%s' of type %i failed to compile, error: %s", shaderFile.c_str(), type, errorMsgBfr);

		return -1u;
	} else {
		this->getLogger().Log(Logger::info, LOG_ARGS, "Shader '%s' of type %i compiled successfully!", shaderFile.c_str(), type);
	}

	this->parseRawShader(shaderText);

    return shader;
} /* ShaderManager::createShader */

void ShaderManager::parseRawShader(std::string &shaderText) {
	size_t strPos = 0;
	const std::string samplerStr = "uniform sampler";
	const std::string layoutStr = "location";

	while (strPos < shaderText.length()) {
		size_t strPosLayout = shaderText.find(layoutStr, strPos);
		size_t strPosSampler = shaderText.find(samplerStr, strPos);

		//strPos = shaderText.find(samplerStr, strPos);
		if (strPosLayout > shaderText.length() && strPosSampler > shaderText.length()) {
			strPos = shaderText.length();
		} else if (strPosSampler < strPosLayout) {
			strPos = strPosSampler;
			//found a uniform sampler
			strPos += samplerStr.length();

			strPos = shaderText.find_first_of(" ", strPos);
			strPos = shaderText.find_first_not_of(" ", strPos);

			size_t tempStrPos = strPos;
			strPos = shaderText.find_first_of(" ;", strPos);

			std::string samplerName = shaderText.substr(tempStrPos, (strPos - tempStrPos));

			DEBUG << "Found a sampler, '" << samplerName << "' now add to map" << std::endl;
			size_t numSamplers = samplerMap.size();
			samplerMap.emplace(samplerName, GL_TEXTURE0 + numSamplers);
		} else {
			strPos = strPosLayout;

			strPos += layoutStr.length();
			strPos = shaderText.find_first_not_of(" =", strPos);

			char num = shaderText.at(strPos);
			GLuint value = num - 48;

			this->layoutList.push_back(value);
			DEBUG << "Adding " << num << " to the list of layouts" << std::endl;
		}
	}
} /* ShaderManager::parseRawShader */

void ShaderManager::initialiseTextureLocations(void) {
	for (std::map<std::string, GLuint>::iterator mapItr = this->samplerMap.begin();
		 mapItr != this->samplerMap.end();
		 mapItr++) {

		GLuint samplerID = this->getUniformLocation(mapItr->first);
		if (samplerID != 0xFFFFFFFF) {
			glUniform1i(samplerID, mapItr->second - GL_TEXTURE0);
		}
	}
} /* ShaderManager::initialiseTextureLocations */

bool ShaderManager::bindTexture(std::string samplerName, GLuint textureID) {
	return this->bindTexture(samplerName, textureID, GL_TEXTURE_2D);
} /* ShaderManager::bindTexture */

bool ShaderManager::bindTexture(std::string samplerName, GLuint textureID, GLenum textureType) {
	if (this->samplerMap.count(samplerName) > 0) {
		GLenum textureUnit = this->samplerMap.at(samplerName);

		glActiveTexture(textureUnit);
		GLuint samplerID = this->getUniformLocation(samplerName);
		glUniform1i(samplerID, textureUnit - GL_TEXTURE0);
		glBindTexture(textureType, textureID);
	} else {
		ERR << "Attempted to bind to unused texture '" << samplerName << "'" << std::endl;

		for (std::map<std::string, GLuint>::iterator mapItr = this->samplerMap.begin();
			 mapItr != this->samplerMap.end();
			 mapItr++) {
			ERR << "Value: " << mapItr->first << std::endl;
		}

		return false;
	}

	return true;
} /* ShaderManager::bindTexture */

bool ShaderManager::bindArray(std::string arrayName, int numArrays, bool someBool, GLfloat *arrayToSet) {
	GLuint matrixID = this->getUniformLocation(arrayName);

	if (matrixID == INACTIVE_SHADER_LOC) {
		return false;
	}

	glUniformMatrix4fv(matrixID, numArrays, someBool, arrayToSet);

	return true;
} /* ShaderManager::bindArray */

bool ShaderManager::bindVector2(std::string vectorName, GLfloat vectorData[2]) {
	glUniform2f(this->getUniformLocation(vectorName), vectorData[0], vectorData[1]);
	return true;
}

bool ShaderManager::bindVector3(std::string vectorName, GLfloat vectorData[3]) {
	glUniform3f(this->getUniformLocation(vectorName), vectorData[0], vectorData[1], vectorData[2]);
	return true;
}

bool ShaderManager::bindVector4(std::string vectorName, GLfloat vectorData[4]) {
	glUniform4f(this->getUniformLocation(vectorName), vectorData[0], vectorData[1], vectorData[2], vectorData[3]);
	return true;
}

GLuint ShaderManager::getShaderProgram(void) {
	return this->shaderProgram;
} /* ShaderManager::getShaderProgram */

GLuint ShaderManager::getLayoutLocation(std::string layoutName) {
	GLint layoutLocation = glGetAttribLocation(this->shaderProgram, layoutName.c_str());

	if (layoutLocation == -1) {
		this->getLogger().Log(Logger::error, LOG_ARGS, "The layout %s is not present in the shader program %s", layoutName.c_str(), this->header.c_str());
	}

	return layoutLocation;
} /* ShaderManager::getLayoutLocation */

GLuint ShaderManager::getUniformLocation(std::string uniformName) {
	GLint uniformLocation = glGetUniformLocation(this->shaderProgram, uniformName.c_str());

	if (uniformLocation == -1) {
		ERR << "The uniform " << uniformName << " is not present in the shader program " << this->header.c_str() << std::endl;
	}

	return uniformLocation;
} /* ShaderManager::getUniformLocation */

void ShaderManager::setUniformFloat(std::string shaderLocation, GLfloat uniformValue) {
	glUniform1f(this->getUniformLocation(shaderLocation), uniformValue);
} /* ShaderManager::setUniformValue */

void ShaderManager::setUniformInt(std::string shaderLocation, GLint uniformValue) {
	glUniform1i(this->getUniformLocation(shaderLocation), uniformValue);
}

void ShaderManager::setUniformBool(std::string shaderLocation, bool uniformValue) {
	glUniform1i(this->getUniformLocation(shaderLocation), uniformValue ? 1 : 0);
}

void ShaderManager::useProgram(void) {
	if (ShaderManager::currentShaderProgram == this) {
		return;
	}

	if (ShaderManager::currentShaderProgram) {
		ShaderManager::currentShaderProgram->disableAttributeLocations();
	}

	glUseProgram(this->shaderProgram);
	ShaderManager::currentShaderProgram = this;
} /* ShaderManager::useProgram */

bool ShaderManager::validateProgram(void) {
	glValidateProgram(this->shaderProgram);

	GLint progStatus;
	glGetProgramiv(this->shaderProgram, GL_VALIDATE_STATUS, &progStatus);

	if (progStatus != GL_TRUE) {
		const int bfrLen = 512;
		char errorMsgBfr[bfrLen];
		glGetProgramInfoLog(this->shaderProgram, bfrLen, NULL, errorMsgBfr);

		this->getLogger().Log(Logger::error, LOG_ARGS, "Shader program %s is invalid, msg: %s", this->header.c_str(), errorMsgBfr);

		return false;
	} else {
		return true;
	}
} /* ShaderManager::validateProgram */

void ShaderManager::enableAttributeLocations(void) {
	for (std::vector<GLuint>::iterator vecItr = this->layoutList.begin();
		 vecItr != this->layoutList.end();
		 vecItr++) {
		glEnableVertexAttribArray(*vecItr);
	}
} /* ShaderManager::enableAttributeLocations */

void ShaderManager::disableAttributeLocations(void) {
	for (std::vector<GLuint>::iterator vecItr = this->layoutList.begin();
		 vecItr != this->layoutList.end();
		 vecItr++) {
		glDisableVertexAttribArray(*vecItr);
	}
} /* ShaderManager::disableAttribuateLocations */

void ShaderManager::Destroy(void) {
    glDeleteProgram(this->shaderProgram);
    this->shaderProgram = -1u;
} /* ShaderManager::Destroy */

ShaderManager::~ShaderManager(void) {
	if (this->shaderProgram != -1u) {
		this->Destroy();
	}
} /* ShaderManager::~ShaderManager */
