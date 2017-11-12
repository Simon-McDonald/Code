/*
 * ShaderManager.cpp
 *
 *  Created on: Oct 13, 2015
 *      Author: Simon
 */

#include "ShaderManager.hpp"

#include <iostream>
#include <fstream>
#include <string>

const Config::ConfigHeader ShaderManager::resourceLoc = "RESOURCES";
ShaderManager* ShaderManager::currentShaderProgram = nullptr;

ShaderManager *ShaderManager::getActiveShaderManager(void) {
	return ShaderManager::currentShaderProgram;
} /* ShaderManager::getActiveShaderManager */

ShaderManager::ShaderManager(Config::ConfigHeader configHeader) :
	header(configHeader), shaderProgram(-1u) {
	// Do not check validity of configHeader, will fail to initialise if configHeader not valid.

	if (!this->InitialiseShader()) {
		throw std::invalid_argument("Argument");
	}
} /* ShaderManager::ShaderManager */

ShaderManager::ShaderManager(void) noexcept : header(""), shaderProgram(-1u) {}

/*ShaderManager::ShaderManager(ShaderManager &&orig) noexcept :

	{

	Config::ConfigHeader header;
	std::map<std::string, GLenum> samplerMap;

	std::vector<GLuint> layoutList;

	GLuint shaderProgram;

}

ShaderManager& ShaderManager::operator=(ShaderManager &&orig) noexcept {

}*/

bool ShaderManager::Initialise(void) {
	return true;
}

bool ShaderManager::InitialiseShader(void) {
	GLuint vertexShader = this->createShader(GL_VERTEX_SHADER, "vertex_shader");
	GLuint tessellationControlShader = this->createShader(GL_TESS_CONTROL_SHADER, "tessellation_control_shader");
	GLuint tessellationEvaluationShader = this->createShader(GL_TESS_EVALUATION_SHADER, "tessellation_evaluation_shader");
	GLuint geometryShader = this->createShader(GL_GEOMETRY_SHADER, "geometry_shader");
	GLuint fragmentShader = this->createShader(GL_FRAGMENT_SHADER, "fragment_shader");

	this->shaderProgram = glCreateProgram();

    glAttachShader(this->shaderProgram, vertexShader);
    glAttachShader(this->shaderProgram, tessellationControlShader);
    glAttachShader(this->shaderProgram, tessellationEvaluationShader);
    glAttachShader(this->shaderProgram, geometryShader);
    glAttachShader(this->shaderProgram, fragmentShader);

    if (this->getConfig().containsKey(this->header, "feedback_variables"))
    {
    	if (fragmentShader == -1u) {
        	this->setFeedbackVaryings();
    	} else {
    		ERR << "The 'feedback_variables' key is present with a fragment shader!";
    	}
    }

    glLinkProgram(this->shaderProgram);

    GLint linkStatus;
    glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &linkStatus);

    if (linkStatus != GL_TRUE) {
		const int bfrLen = 512;
		char errorMsgBfr[bfrLen];
		glGetProgramInfoLog(this->shaderProgram, bfrLen, NULL, errorMsgBfr);

		ERR << "Failed to link shader program '" << this->header << "', error: " << errorMsgBfr << END;

    	return false;
    } else {
    	INFO << "Linked shader program " << this->header.c_str() << END;
    }

	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	this->useProgram();

	this->initialiseTextureLocations();

    return true;
} /* ShaderManager::Initialise */

void ShaderManager::setFeedbackVaryings(void) {
	std::vector<std::string> varyings;
	this->getConfig().getVector(this->header, "feedback_variables", varyings);

	const GLchar *feedbackVaryings[10] = {0};

	for (unsigned vary_idx = 0; vary_idx < varyings.size(); ++vary_idx) {
		INFO << "Adding varying '" << varyings.at(vary_idx) << "' in shader '" << this->header << "'" << END;
		feedbackVaryings[vary_idx] = varyings.at(vary_idx).data();
	}

	glTransformFeedbackVaryings(this->shaderProgram, varyings.size(), feedbackVaryings, GL_INTERLEAVED_ATTRIBS);
}

GLuint ShaderManager::createShader(GLenum type, Config::ConfigKey key) {
	if (!this->getConfig().containsHeader(ShaderManager::resourceLoc) ||
	    !this->getConfig().containsKey(ShaderManager::resourceLoc, "shader_dir")) {
		//this->getLogger().Log(Logger::info, LOG_ARGS, "No shader folder path");
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
		//this->getLogger().Log(Logger::error, LOG_ARGS,
			//"Cannot open shader %s!", shaderFile.c_str());
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
		INFO << "Shader '" << shaderFile << "' of type " << type << " compiled successfully!" << END;
	}

	this->parseRawShader(shaderText);

	if (type == GL_TESS_CONTROL_SHADER) {
		GLint MaxPatchVertices = 0;
		glGetIntegerv(GL_MAX_PATCH_VERTICES, &MaxPatchVertices);
		INFO << "Max supported patch vertices " << MaxPatchVertices << END;
		glPatchParameteri(GL_PATCH_VERTICES, 3);
	}

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

			//size_t tempStrPos = shaderText.find_first_of(" )", strPos);

			//std::string layoutNumStr = shaderText.substr(strPos, (tempStrPos - strPos));

			char num = shaderText.at(strPos);
			GLuint value = num - 48;
			//int num = stoi(layoutNumStr);
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
		} else {
			ERR << "Trying to get a texture that does not exist " << mapItr->first << END;
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
		ERR << "Attempted to bind to unused texture '" << samplerName << "'" << END;

		for (std::map<std::string, GLuint>::iterator mapItr = this->samplerMap.begin();
			 mapItr != this->samplerMap.end();
			 mapItr++) {
			ERR << "Value: " << mapItr->first << std::endl;
		}

		return false;
	}

	return true;
} /* ShaderManager::bindTexture */

bool ShaderManager::bindTextureArray(std::string samplerName, GLuint numTextures, GLuint *textureIDs, GLuint startUnitID) {

	GLint shaderValues[numTextures];
	for (GLuint offset = 0; offset < numTextures; ++offset) {
		shaderValues[offset] = startUnitID + offset;
	}

	GLuint samplerID = this->getUniformLocation(samplerName);
	glUniform1iv(samplerID, numTextures, &shaderValues[0]);


	for (GLuint offset = 0; offset < numTextures; ++offset) {
		glActiveTexture(GL_TEXTURE0 + startUnitID + offset);
		glBindTexture(GL_TEXTURE_2D, textureIDs[offset]);
	}

	return true;
}

bool ShaderManager::bindArray(std::string arrayName, int numArrays, bool someBool, GLfloat *arrayToSet) {
	GLuint matrixID = this->getUniformLocation(arrayName);

	if (matrixID == INACTIVE_SHADER_LOC) {
		return false;
	}

	glUniformMatrix4fv(matrixID, numArrays, someBool, arrayToSet);

	return true;
} /* ShaderManager::bindArray */

GLuint ShaderManager::getShaderProgram() {
	return this->shaderProgram;
} /* ShaderManager::getShaderProgram */

GLuint ShaderManager::getLayoutLocation(std::string layoutName) {
	GLint layoutLocation = glGetAttribLocation(this->shaderProgram, layoutName.c_str());

	if (layoutLocation == -1) {
		//this->getLogger().Log(Logger::error, LOG_ARGS, "The layout %s is not present in the shader program %s", layoutName.c_str(), this->header.c_str());
	}

	return layoutLocation;
} /* ShaderManager::getLayoutLocation */

GLuint ShaderManager::getUniformLocation(std::string uniformName) {
	GLint uniformLocation = glGetUniformLocation(this->shaderProgram, uniformName.c_str());

	if (uniformLocation == -1) {
		//this->getLogger().Log(Logger::error, LOG_ARGS, "The uniform %s is not present in the shader program %s", uniformName.c_str(), this->header.c_str());
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

bool ShaderManager::bindVector2(std::string vectorName, GLfloat vectorData[2]) {
	glUniform2f(this->getUniformLocation(vectorName), vectorData[0], vectorData[1]);
	return true;
}

bool ShaderManager::bindVector2(std::string vectorName, GLfloat vectorData0, GLfloat vectorData1) {
	glUniform2f(this->getUniformLocation(vectorName), vectorData0, vectorData1);
	return true;
}

void ShaderManager::bindVector2i(std::string vectorName, GLint vectorData[2]) {
	glUniform2i(this->getUniformLocation(vectorName), vectorData[0], vectorData[1]);
}

void ShaderManager::bindVector2i(std::string vectorName, GLint vectorData0, GLint vectorData1) {
	glUniform2i(this->getUniformLocation(vectorName), vectorData0, vectorData1);
}

bool ShaderManager::bindVector3(std::string vectorName, GLfloat vectorData[3]) {
	glUniform3f(this->getUniformLocation(vectorName), vectorData[0], vectorData[1], vectorData[2]);
	return true;
}

bool ShaderManager::bindVector2iv(std::string vectorName, size_t vecLength, GLint *vector) {
	glUniform2iv(this->getUniformLocation(vectorName), vecLength, vector);
	return true;
}

bool ShaderManager::bindVector3i(std::string vectorName, GLuint vectorData0, GLuint vectorData1, GLuint vectorData2) {
	glUniform3i(this->getUniformLocation(vectorName), vectorData0, vectorData1, vectorData2);
	return true;
}

bool ShaderManager::bindVector4(std::string vectorName, GLfloat vectorData[4]) {
	glUniform4f(this->getUniformLocation(vectorName), vectorData[0], vectorData[1], vectorData[2], vectorData[3]);
	return true;
}

void ShaderManager::useProgram() {
	if (ShaderManager::currentShaderProgram == this) {
		return;
	}

	if (ShaderManager::currentShaderProgram) {
		//ShaderManager::currentShaderProgram->disableAttributeLocations();
	}

	glUseProgram(this->shaderProgram);
	ShaderManager::currentShaderProgram = this;
} /* ShaderManager::useProgram */

bool ShaderManager::validateProgram() {
	glValidateProgram(this->shaderProgram);

	GLint progStatus;
	glGetProgramiv(this->shaderProgram, GL_VALIDATE_STATUS, &progStatus);

	if (progStatus != GL_TRUE) {
		const int bfrLen = 512;
		char errorMsgBfr[bfrLen];
		glGetProgramInfoLog(this->shaderProgram, bfrLen, NULL, errorMsgBfr);

		ERR << "Shader program " << this->header << " is invalid, msg: " << errorMsgBfr << END;

		return false;
	} else {
		return true;
	}
} /* ShaderManager::validateProgram */

void ShaderManager::enableAttributeLocations(void) {
	for (std::vector<GLuint>::iterator vecItr = this->layoutList.begin();
		 vecItr != this->layoutList.end();
		 vecItr++) {
		//DEBUG << "Enable number " << *vecItr << std::endl;
		glEnableVertexAttribArray(*vecItr);
	}
} /* ShaderManager::enableAttributeLocations */

void ShaderManager::disableAttributeLocations(void) {
	for (std::vector<GLuint>::iterator vecItr = this->layoutList.begin();
		 vecItr != this->layoutList.end();
		 vecItr++) {
		//DEBUG << "Disable number " << *vecItr << std::endl;
		glDisableVertexAttribArray(*vecItr);
	}
} /* ShaderManager::disableAttribuateLocations */

void ShaderManager::Destroy(void) {
	//Obsolete
	//glDeleteProgram(this->shaderProgram);
} /* ShaderManager::Destroy */

ShaderManager::~ShaderManager(void) {
	glDeleteProgram(this->shaderProgram);
	ERR << "Destroying a shader manager" << END;
} /* ShaderManager::~ShaderManager */
