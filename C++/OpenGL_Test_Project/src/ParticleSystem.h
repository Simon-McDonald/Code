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
	ParticleSystem(
			glm::vec3 launcherPosition,
			glm::vec3 launcherOrientation,
			float launcherPower,
			float launcherPeriod_s,
			float launcherTimeOffset_s,
			float primaryShellLifetime_s,
			int numSecondaryShells,
			float secondaryShellLifetime_s);
	~ParticleSystem();

	bool InitParticleSystem(void);

	void Render(float deltaTime_sec, const glm::mat4 viewProjMat, const glm::vec3 cameraPos);

private:
	typedef struct {
		glm::vec3 launcherPosition;
		glm::vec3 launcherOrientation;
		float launcherPower;
		float launcherPeriod_s;
		float launcherTimeOffset_s;
		float primaryShellLifetime_s;
		int numSecondaryShells;
		float secondaryShellLifetime_s;
	} LauncherCharacteristics;

	typedef struct {
		float type;
		glm::vec3 pos;
		glm::vec3 vel;
		float lifetimeMillis;
	} Particle;

	bool isFirst;
	unsigned int currVB;
	unsigned int currTFB;
	GLuint particleBuffer[2];
	GLuint transformFeedback[2];
	GLuint randomTextureID;
	float timeVal;
	BillboardManager billboardImage;
	LauncherCharacteristics launcherSpecs;

	void updateParticles(float deltaTime_sec);
	void renderParticles(const glm::mat4 viewProjMat, const glm::vec3 cameraPos);
	bool InitRandomTexture(unsigned int Size);
};

#endif /* PARTICLESYSTEM_H_ */
