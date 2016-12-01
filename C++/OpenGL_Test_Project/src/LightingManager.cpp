/*
 * LightingManager.cpp
 *
 *  Created on: Jun 25, 2016
 *      Author: Simon
 */

#include <iostream>

#include "LightingManager.h"

float LightingManager::matSpecularIntensity = 0;
float LightingManager::specularPower = 0;

LightingManager::LightingManager(std::string shaderLocName, glm::vec3 lightColour, float ambientIntensity, float diffuseIntensity) :
	shaderLocName(shaderLocName), lightColour(lightColour), ambientIntensity(ambientIntensity), diffuseIntensity(diffuseIntensity) {
} /* LightingManager::LightingManager */

void LightingManager::initGlobalShaderParams(float matSpecularIntensity, float specularPower) {
	LightingManager::matSpecularIntensity = matSpecularIntensity;
	LightingManager::specularPower = specularPower;
} /* LightingManager::initGlobalShaderParams */

void LightingManager::setGlobalShaderParams(void) {
	GLuint matSpecularIntensityID = LightingManager::getShaderManager().getUniformLocation("gMatSpecularIntensity");
	GLuint specularPowerID = LightingManager::getShaderManager().getUniformLocation("gSpecularPower");

	if (matSpecularIntensityID == INACTIVE_SHADER_LOC ||
		specularPowerID == INACTIVE_SHADER_LOC) {

		std::cout << "some static light things not used" << std::endl;
	}

	glUniform1f(matSpecularIntensityID, LightingManager::matSpecularIntensity);
	glUniform1f(specularPowerID, LightingManager::specularPower);
} /* LightingManager::setGlobalShaderParams */

void LightingManager::setLightInShader(void) {
	GLuint colourID = this->getShaderManager().getUniformLocation(this->shaderLocName + ".Base.Color");
	GLuint ambientIntensityID = this->getShaderManager().getUniformLocation(this->shaderLocName + ".Base.AmbientIntensity");
	GLuint diffuseIntensityID = this->getShaderManager().getUniformLocation(this->shaderLocName + ".Base.DiffuseIntensity");

	if (colourID == INACTIVE_SHADER_LOC ||
		ambientIntensityID == INACTIVE_SHADER_LOC ||
		diffuseIntensityID == INACTIVE_SHADER_LOC) {

		std::cout << "Warning: some lighting stuff is not initialised" << std::endl;
	}

	glUniform3f(colourID, this->lightColour.x, this->lightColour.y, this->lightColour.z);
	glUniform1f(ambientIntensityID, this->ambientIntensity);
	glUniform1f(diffuseIntensityID, this->diffuseIntensity);
} /* LightingManager::setLightInShader */

LightingManager::~LightingManager(void) {
} /* LightingManager::~LightingManager */
