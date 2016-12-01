/*
 * ParticleSystem.h
 *
 *  Created on: Jul 31, 2016
 *      Author: Simon
 */

#ifndef PARTICLESYSTEM_H_
#define PARTICLESYSTEM_H_

#include "ResourceManager.h"
#include "BillboardManager.h"

class ParticleSystem : protected ResourceManager {
public:
	ParticleSystem();
	~ParticleSystem();

	bool InitParticleSystem(const glm::vec3 pos);

	void Render(float deltaTime_sec, const glm::mat4 viewProjMat, const glm::vec3 cameraPos);

private:
	bool isFirst;
	unsigned int currVB;
	unsigned int currTFB;
	GLuint particleBuffer[2];
	GLuint transformFeedback[2];
	GLuint randomTextureID;
	float timeVal;
	BillboardManager billboardImage;

	void updateParticles(float deltaTime_sec);
	void renderParticles(const glm::mat4 viewProjMat, const glm::vec3 cameraPos);
	bool InitRandomTexture(unsigned int Size);
};

#endif /* PARTICLESYSTEM_H_ */
