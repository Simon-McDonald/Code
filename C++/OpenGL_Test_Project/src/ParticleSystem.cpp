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

struct Particle {
	float type;
	glm::vec3 pos;
	glm::vec3 vel;
	float lifetimeMillis;
};

ParticleSystem::ParticleSystem(void) :
		isFirst(true), currVB(0), currTFB(1),
		particleBuffer{-1u, -1u}, transformFeedback{-1u, -1u},
		randomTextureID(-1u),
		timeVal(0),
		billboardImage("bang.bmp") {}

bool ParticleSystem::InitParticleSystem(const glm::vec3 pos) {
	struct Particle particles[MAX_PARTICLES];
	memset(particles, 0, sizeof (particles));

	particles[0].type = PARTICLE_TYPE_LAUNCHER;
	particles[0].pos = pos;
	particles[0].vel = glm::vec3(0.0f, 0.0001f, 0.0f);
	particles[0].lifetimeMillis = 100.0f + pos.x * 50.0 + pos.y * 50.0 + pos.y * 50.0;

	glGenTransformFeedbacks(2, this->transformFeedback);
	glGenBuffers(2, this->particleBuffer);

	for (unsigned int i = 0; i < 2; i++) {
		glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->transformFeedback[i]);
		glBindBuffer(GL_ARRAY_BUFFER, this->particleBuffer[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, this->particleBuffer[i]);
	}

	this->loadTexture("bang.bmp");
	this->InitRandomTexture(100);
	// something else
	return true;
}

void ParticleSystem::Render(float deltaTime_sec, const glm::mat4 viewProjMat, const glm::vec3 cameraPos) {
	this->timeVal += deltaTime_sec;

	this->updateParticles(deltaTime_sec);
	CHECKERRORS();
	this->renderParticles(viewProjMat, cameraPos);
	CHECKERRORS();
	this->currVB = this->currTFB;
	this->currTFB = (this->currTFB + 1) & 0x1;
}

void ParticleSystem::updateParticles(float deltaTime_sec) {

	this->getShaderManager("Transform").useProgram();

	this->getShaderManager().setUniformInt("gExplosionPairs", 20);
	this->getShaderManager().setUniformFloat("gLauncherPower", 0.08);
	glm::vec3 orientation = glm::vec3(0.0, 1.0, 5.0);
	this->getShaderManager().bindVector3("gLauncherOrientation", &orientation[0]);
	this->getShaderManager().setUniformFloat("gDeltaTimeMillis", deltaTime_sec * 1000);
	this->getShaderManager().setUniformFloat("gLauncherLifetime", 1000);
	CHECKERRORS();
	this->getShaderManager().setUniformFloat("gShellLifetime", 1500);
	this->getShaderManager().setUniformFloat("gSecondaryShellLifetime", 1500);
	this->getShaderManager().setUniformFloat("gTime", this->timeVal * 1000);
	CHECKERRORS();
	//this->getShaderManager().bindTexture("gRandomTexture", this->randomTextureID);
	CHECKERRORS();
	glEnable(GL_RASTERIZER_DISCARD);

	glBindBuffer(GL_ARRAY_BUFFER, this->particleBuffer[this->currVB]);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, this->transformFeedback[this->currTFB]);
	CHECKERRORS();
	this->getShaderManager().enableAttributeLocations();
	CHECKERRORS();
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(struct Particle), 0); //type;
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(struct Particle), (const GLvoid*) 4); //position
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(struct Particle), (const GLvoid*) 16); //velocity
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(struct Particle), (const GLvoid*) 28); //lifetime
	CHECKERRORS();
	glBeginTransformFeedback(GL_POINTS);

	if (this->isFirst) {
		glDrawArrays(GL_POINTS, 0, 1);
		this->isFirst = false;
	} else {
		glDrawTransformFeedback(GL_POINTS, this->transformFeedback[this->currVB]);
	}
	CHECKERRORS();
	glEndTransformFeedback();
	CHECKERRORS();
	this->getShaderManager().disableAttributeLocations();
	glDisable(GL_RASTERIZER_DISCARD);
}

void ParticleSystem::renderParticles(glm::mat4 viewProjMat, glm::vec3 cameraPos) {
	this->getShaderManager("Billboard").useProgram();

	this->getShaderManager().bindArray("gVP", 1, GL_FALSE, &viewProjMat[0][0]);
	this->setTexture("bang.bmp");

	glUniform3f(this->getShaderManager().getUniformLocation("gCameraPos"), cameraPos.x, cameraPos.y, cameraPos.z);

	glBindBuffer(GL_ARRAY_BUFFER, this->particleBuffer[this->currTFB]);

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(struct Particle), (const GLvoid*)4);
	glDrawTransformFeedback(GL_POINTS, this->transformFeedback[this->currTFB]);

	glDisableVertexAttribArray(0);
}

bool ParticleSystem::InitRandomTexture(unsigned int Size) {
	glm::vec3* pRandomData = new glm::vec3[Size];

	srand((unsigned int) time(NULL));

	for (unsigned int i = 0 ; i < Size ; i++) {
		pRandomData[i].x = ((float) rand()) / RAND_MAX;
		pRandomData[i].y = ((float) rand()) / RAND_MAX;
		pRandomData[i].z = ((float) rand()) / RAND_MAX;
	}

	glGenTextures(1, &this->randomTextureID);
	glBindTexture(GL_TEXTURE_1D, this->randomTextureID);
	glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, Size, 0.0f, GL_RGB, GL_FLOAT, pRandomData);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	delete [] pRandomData;

	return (glGetError() != GL_NO_ERROR);
}

ParticleSystem::~ParticleSystem() {}
