/*
 * ParticleSystem.cpp
 *
 *  Created on: Jul 31, 2016
 *      Author: Simon
 */

#include <stdlib.h>
#include <ctime>

#include "ParticleSystem.h"

#include "CheckErrors.h"

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

#define MAX_PARTICLES 100

ParticleSystem::ParticleSystem(
	glm::vec3 launcherPosition,
	glm::vec3 launcherOrientation,
	float launcherPower,
	float launcherPeriod_s,
	float launcherTimeOffset_s,
	float primaryShellLifetime_s,
	int numSecondaryShells,
	float secondaryShellLifetime_s) :
	isFirst(true), currVB(0), currTFB(1),
	particleBuffer{-1u, -1u}, transformFeedback{-1u, -1u},
	/*randomTextureID(-1u),*/
	timeVal(0),
	billboardImage("bang.bmp"),
	launcherSpecs({launcherPosition,
		launcherOrientation,
		launcherPower,
		launcherPeriod_s,
		launcherTimeOffset_s,
		primaryShellLifetime_s,
		numSecondaryShells,
		secondaryShellLifetime_s}) {
}

bool ParticleSystem::InitParticleSystem(void) {
	Particle particles[MAX_PARTICLES];
	memset(particles, 0, sizeof (particles));

	particles[0].type = PARTICLE_TYPE_LAUNCHER;
	particles[0].pos = this->launcherSpecs.launcherPosition;
	particles[0].vel = glm::vec3(0.0f, 0.0f, 0.0f);
	particles[0].lifetimeMillis = this->launcherSpecs.launcherTimeOffset_s * 1000;

	glGenTransformFeedbacks(2, this->transformFeedback);
	glGenBuffers(2, this->particleBuffer);

	for (unsigned int i = 0; i < 2; i++) {
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->transformFeedback[i]);
		glBindBuffer(GL_ARRAY_BUFFER, this->particleBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->particleBuffer[i]);
	}

	this->loadTexture("bang.bmp");
	this->InitRandomTexture(1024);

	return true;
}

void ParticleSystem::Render(float deltaTime_sec, const glm::mat4 viewProjMat, const glm::vec3 cameraPos) {
	this->timeVal += deltaTime_sec;

	this->updateParticles(deltaTime_sec);
	this->renderParticles(viewProjMat, cameraPos);

	this->currVB = this->currTFB;
	this->currTFB = (this->currTFB + 1) & 0x1;
	CHECKERRORS();
}

void ParticleSystem::updateParticles(float deltaTime_sec) {
	this->getShaderManager("Transform").useProgram();

	this->getShaderManager().setUniformFloat("gDeltaTimeMillis", deltaTime_sec * 1000);
	this->getShaderManager().setUniformFloat("gTime", this->timeVal * 1000);

	this->getShaderManager().bindVector3("gLauncherOrientation", &this->launcherSpecs.launcherOrientation[0]);
	this->getShaderManager().setUniformFloat("gLauncherPower", this->launcherSpecs.launcherPower);
	this->getShaderManager().setUniformFloat("gLauncherLifetime", this->launcherSpecs.launcherPeriod_s *  1000);
	this->getShaderManager().setUniformFloat("gShellLifetime", this->launcherSpecs.primaryShellLifetime_s * 1000);
	this->getShaderManager().setUniformInt("gExplosionPairs", this->launcherSpecs.numSecondaryShells / 2);
	this->getShaderManager().setUniformFloat("gSecondaryShellLifetime", this->launcherSpecs.secondaryShellLifetime_s * 1000);

	this->getShaderManager().bindTexture("gRandomTexture", this->randomTexture.getBufferID(), GL_TEXTURE_1D);

	glEnable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, this->particleBuffer[this->currVB]);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->transformFeedback[this->currTFB]);

	this->getShaderManager().enableAttributeLocations();

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), 0); //type;
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*) 4); //position
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*) 16); //velocity
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*) 28); //lifetime

	glBeginTransformFeedback(GL_POINTS);

	if (this->isFirst) {
		glDrawArrays(GL_POINTS, 0, 1);
		this->isFirst = false;
	} else {
		glDrawTransformFeedback(GL_POINTS, this->transformFeedback[this->currVB]);
	}

	glEndTransformFeedback();
	this->getShaderManager().disableAttributeLocations();
	glDisable(GL_RASTERIZER_DISCARD);
}

void ParticleSystem::renderParticles(glm::mat4 viewProjMat, glm::vec3 cameraPos) {
	this->getShaderManager("Billboard").useProgram();

	this->getShaderManager().bindArray("gVP", 1, GL_FALSE, &viewProjMat[0][0]);
	this->setTexture("bang.bmp");
	glUniform3f(this->getShaderManager().getUniformLocation("gCameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	glBindBuffer(GL_ARRAY_BUFFER, this->particleBuffer[this->currTFB]);

	this->getShaderManager().enableAttributeLocations();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (const GLvoid*)4);

	glDrawTransformFeedback(GL_POINTS, this->transformFeedback[this->currTFB]);

	this->getShaderManager().disableAttributeLocations();
}

bool ParticleSystem::InitRandomTexture(unsigned int Size) {
	glm::vec3* pRandomData = new glm::vec3[Size];

	srand((unsigned int) time(NULL));

	for (unsigned int i = 0 ; i < Size ; i++) {
		pRandomData[i].x = ((float) rand()) / RAND_MAX;
		pRandomData[i].y = ((float) rand()) / RAND_MAX;
		pRandomData[i].z = ((float) rand()) / RAND_MAX;
	}

	this->randomTexture.initTextureManager(Size, pRandomData, GL_FLOAT);

	//this->randomTextureID = this->randomTexture.getBufferID();

	delete [] pRandomData;

	return (glGetError() != GL_NO_ERROR);
}

ParticleSystem::~ParticleSystem() {}
