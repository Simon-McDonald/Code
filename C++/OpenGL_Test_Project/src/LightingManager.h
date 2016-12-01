/*
 * LightingManager.h
 *
 *  Created on: Jun 25, 2016
 *      Author: Simon
 */

#ifndef LIGHTINGMANAGER_H_
#define LIGHTINGMANAGER_H_

#include <glm/glm.hpp>

#include "UtilityManager.h"
#include "WorldManager.h"

class LightingManager : protected UtilityManager, protected WorldManager {
public:
	LightingManager(std::string shaderLocName, glm::vec3 lightColour, float ambientIntensity, float diffuseIntensity);
	static void initGlobalShaderParams(float matSpecularIntensity, float specularPower);

	virtual void setLightInShader(void);
	static void setGlobalShaderParams(void);

	virtual ~LightingManager(void);

protected:
	std::string shaderLocName;

private:
	static float matSpecularIntensity;
	static float specularPower;

	glm::vec3 lightColour;
	float ambientIntensity;
	float diffuseIntensity;
};

#endif /* LIGHTINGMANAGER_H_ */
