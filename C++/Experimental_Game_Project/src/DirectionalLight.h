/*
 * DirectionalLight.h
 *
 *  Created on: Jun 26, 2016
 *      Author: Simon
 */

#ifndef DIRECTIONALLIGHT_H_
#define DIRECTIONALLIGHT_H_

#include "LightingManager.h"

class DirectionalLight : public LightingManager {
public:
	DirectionalLight(std::string shaderLocName, glm::vec3 lightColour, float ambientIntensity, float diffuseIntensity, glm::vec3 direction);

	virtual void setLightInShader(void);

	virtual ~DirectionalLight(void);

private:

private:
	glm::vec3 direction;

};

#endif /* DIRECTIONALLIGHT_H_ */
