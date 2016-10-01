/*
 * Player.h
 *
 *  Created on: Jun 12, 2016
 *      Author: Simon
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "ResourceManager.h"

class Player : protected ResourceManager {
public:
	Player(void);

	bool InitialisePlayer(
		std::vector<std::string> &resourcesVector);

	void Update(float deltaTime, std::vector<std::vector<char>> &worldArray);

	void Render(void);

	~Player();

private:
	static const float accelerationRate;
	static const float decelerationRate;
	static const float maxSpeed;
	static const float jumpAcceleration;
	static const float gravity;
	static const float jumpAccelerationTime;

	glm::vec2 position;
	glm::vec2 velocity;

	bool jumping;
	float actionDuration;
	bool facingForward;
	int runNum;

	GLuint textureID;
	GLuint pointBufferID;
	GLuint textureGridDim;

	void HandleMovementUpdates(float deltaTime, std::vector<std::vector<char>> &worldArray);
	void HandleInputUpdate(float deltaTime);
	float getIntersectionTime(float pos, float vel);
	glm::vec2 getIntersectionTimes(glm::vec2 &pos, glm::vec2 &vel);
	bool checkCollisionsOnLine(std::vector<std::vector<char>> &worldArray, int lengthIdx,
			std::vector<int> &idxsToCheck, bool isVerticalLine);
};

#endif /* PLAYER_H_ */
