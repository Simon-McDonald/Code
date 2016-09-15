/*
 * Player.h
 *
 *  Created on: Jun 12, 2016
 *      Author: Simon
 */

#ifndef PLAYER_H_
#define PLAYER_H_

#include "RenderableObject.h"

class Player : public RenderableObject {
public:
	Player(glm::vec3 initPosition, float scale);

	virtual void Update(float deltaTime);

	glm::mat4 getViewMatrix(void);
	glm::mat4 getRotationMatrix(void);

	~Player();

	/*float horizontalAngle;
	float verticalAngle;*/

private:
	static const float translationRate;
	static const float rotationRate;

	glm::vec3 velocity;
	float jumpDuration;
	bool jumpAnimation;

	float fieldView;

public:
	float horizontalAngle;
	float verticalAngle;
};

#endif /* PLAYER_H_ */
