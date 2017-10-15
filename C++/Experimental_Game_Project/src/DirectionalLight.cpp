/*
 * DirectionalLight.cpp
 *
 *  Created on: Jun 26, 2016
 *      Author: Simon
 */

#include <iostream>

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(std::string shaderLocName, glm::vec3 lightColour, float ambientIntensity, float diffuseIntensity, glm::vec3 direction) :
	LightingManager(shaderLocName, lightColour, ambientIntensity, diffuseIntensity), direction(glm::normalize(direction)) {
} /* DirectionalLight::DirectionalLight */

void DirectionalLight::setLightInShader(void) {
	LightingManager::setLightInShader();

	GLuint directionID = this->getShaderManager().getUniformLocation(this->shaderLocName + ".Direction");

	if (directionID == INACTIVE_SHADER_LOC) {
		std::cout << "Warning: some lighting stuff is not initialised" << std::endl;
	}

	glUniform3f(directionID, this->direction.x, this->direction.y, this->direction.z);
} /* DirectionalLight::setLightInShader */

DirectionalLight::~DirectionalLight(void) {
} /* DirectionalLight::~DirectionalLight */
