/*
 * Player.cpp
 *
 *  Created on: Jun 12, 2016
 *      Author: Simon
 */

#include <limits.h>

#include "CheckErrors.h"
#include "Player.h"

const float Player::accelerationRate = 0.2f;
const float Player::decelerationRate = 0.5f;
const float Player::maxSpeed = 1.0f;
const float Player::jumpAcceleration = 120.0f;
const float Player::gravity = 70.0f;
const float Player::jumpAccelerationTime = 0.3f;

Player::Player(void) :
		position(1.5, 1.0),
		velocity(0.0, 0.0),
		jumping(false),
		actionDuration(0.0f),
		facingForward(true),
		runNum(0),
		textureID(-1u),
		pointBufferID(-1u),
		textureGridDim(-1u) {
}

bool Player::InitialisePlayer(
	std::vector<std::string> &resourcesVector) {

	this->textureID = this->generateTexture(resourcesVector);

	DEBUG << "Player texture ID: " << this->textureID << std::endl;

	this->textureGridDim = this->determineTextureGridSize(resourcesVector.size());

	std::vector<GLfloat> ha = {0.0, 1.0, 2.0, 3.0};

	glGenBuffers(1, &pointBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, pointBufferID);
	glBufferData(GL_ARRAY_BUFFER, ha.size() * sizeof(GLfloat), ha.data(), GL_STATIC_DRAW);

	return true;
}

float Player::getIntersectionTime(float pos, float vel) {
	float time = std::numeric_limits<float>::infinity();

	if (vel != 0) {
		float boundaryOffset = pos - (long) pos;


		boundaryOffset = (boundaryOffset != 0 && vel > 0) ? (1.0 - boundaryOffset) : boundaryOffset;

		time = fabs(boundaryOffset / vel);
		INFO << "offset: " << boundaryOffset << ", time: " << time << std::endl;
	}

	INFO << "POS: " << pos << ", VEL: " << vel << ", TIME: " << time << std::endl;
	return time;
}

glm::vec2 Player::getIntersectionTimes(glm::vec2 &pos, glm::vec2 &vel) {
	return glm::vec2(this->getIntersectionTime(pos.x, vel.x),
					this->getIntersectionTime(pos.y, vel.y));
}

bool Player::checkCollisionsOnLine(std::vector<std::vector<char>> &worldArray, int lengthIdx,
		std::vector<int> &idxsToCheck, bool isVerticalLine) {
	bool clear = true;

	for (std::vector<int>::iterator itr = idxsToCheck.begin();
			clear && (itr != idxsToCheck.end());
			++itr) {
		int rowIdx = isVerticalLine ? lengthIdx : *itr;
		int colIdx = isVerticalLine ? *itr : lengthIdx;

		clear &= worldArray
				.at(rowIdx)
				.at(colIdx) == ' ';

		DEBUG << "Testing (" << (rowIdx) << ", " << (colIdx) << ") - '"
				<< worldArray.at(rowIdx).at(colIdx) << "'" << std::endl;
	}

	return clear;
}

void Player::HandleMovementUpdates(float deltaTime, std::vector<std::vector<char>> &worldArray) {
	float analysedTimeLeft = deltaTime;
	bool hitTwinBoundary = false;

	while (analysedTimeLeft > 0) {
		INFO << "The initial position at the start of the loop: (" << this->position.x << ", " << this->position.y << ")" << std::endl;
		INFO << "The velocities at the start of the loop: (" << this->velocity.x << ", " << this->velocity.y << ")" << std::endl;

		glm::vec2 intersectionTimes = this->getIntersectionTimes(this->position, this->velocity);

		INFO << "The intersection times are (" << intersectionTimes.x << ", " << intersectionTimes.y << ")" << std::endl;

		// no more intersections, do move and end.
		if ((intersectionTimes[0] > analysedTimeLeft) && (intersectionTimes[1] > analysedTimeLeft)) {
			this->position += this->velocity * analysedTimeLeft;
			analysedTimeLeft = 0;
			INFO << "There were no collisions, new position: (" << this->position.x << ", " << this->position.y << ")" << std::endl;
			break;
		}

		bool hitVerticalBoundary = (intersectionTimes[0] < intersectionTimes[1]);

		if (hitTwinBoundary && !hitVerticalBoundary) {
			hitVerticalBoundary = true;
		}

		if (hitVerticalBoundary) {
			INFO << "HitVerticalBoundary is TRUE" << std::endl;
		} else {
			INFO << "HitVerticalBoundary is FALSE" << std::endl;
		}

		int primaryDirPos = hitVerticalBoundary ? this->position.x : this->position.y;
		int secondaryDirPos = hitVerticalBoundary ? this->position.y : this->position.x;

		std::vector<int> perpIndicesToCheck = {secondaryDirPos};
		if ((float) secondaryDirPos != (hitVerticalBoundary ? this->position.y : this->position.x)) {
			perpIndicesToCheck.push_back(secondaryDirPos + 1);
		}

		float primaryVelocity = hitVerticalBoundary ? this->velocity.x : this->velocity.y;

		int borderRowCol = primaryDirPos;
		if (primaryVelocity < 0) {
			borderRowCol -= 1;
		} else if (primaryVelocity > 0) {
			if ((float) primaryDirPos != (hitVerticalBoundary ? this->position.x : this->position.y)) {
				borderRowCol += 2;
			} else {
				borderRowCol += 1;
			}
		}
		INFO << "primaryDirPos: " << primaryDirPos << ", borderRowCol: " << borderRowCol << std::endl;

		if (this->checkCollisionsOnLine(worldArray, borderRowCol, perpIndicesToCheck, hitVerticalBoundary)) {
			INFO << "Was clear" << std::endl;
			// can move into next row/column
			float nextBoundaryTime = (hitVerticalBoundary ? intersectionTimes[1] : intersectionTimes[0]);

			if (nextBoundaryTime > analysedTimeLeft) {
				this->position += this->velocity * analysedTimeLeft;
				analysedTimeLeft = 0;
			} else {
				this->position += this->velocity * nextBoundaryTime;
				analysedTimeLeft -= nextBoundaryTime;

				if (hitVerticalBoundary) {
					this->position.y = round(this->position.y);
				} else {
					this->position.x = round(this->position.x);
				}
			}

			if (intersectionTimes[0] == 0.0 && intersectionTimes[1] == 0.0) {
				if (!hitVerticalBoundary) {
					INFO << "Got the first part of a twin boundary, hitVerticalBoundary is: " << hitVerticalBoundary << std::endl;
					hitTwinBoundary = true;
				} else {
					INFO << "Got the second part of a twin boundary, hitVerticalBoundary is: " << hitVerticalBoundary << std::endl;
					this->position.x += (this->velocity.x > 0) ? 0.1 : -0.1;
					this->position.y += (this->velocity.y > 0) ? 0.1 : -0.1;

					hitTwinBoundary = false;
				}
			} else {
				hitTwinBoundary = false;
			}

		} else {
			INFO << "Was not clear" << std::endl;
			float boundaryTime = (hitVerticalBoundary ? intersectionTimes[0] : intersectionTimes[1]);

			this->position += this->velocity * boundaryTime;
			analysedTimeLeft -= boundaryTime;

			// align the object to the row/col
			/*if (hitVerticalBoundary) {
				this->position.x = ;
			} else
				this->position.y = round(this->position.x);
			}*/

			if (hitVerticalBoundary) {
				this->velocity.x = 0;
			} else {
				if (this->velocity.y < 0) {
					this->jumping = false;
				}

				this->velocity.y = 0;
			}
			hitTwinBoundary = false;
		}
	}
}

void Player::HandleInputUpdate(float deltaTime) {
	INFO << "Game loop UPDATE!!!!!!!!!!" << std::endl;

 	UserInputStruct &input = this->getWindow().getInput();

	// Move forward
	if (input.up){
		if (!this->jumping) {
			this->jumping = true;
			this->actionDuration = Player::jumpAccelerationTime;
		}
		if (this->actionDuration > 0) {
			if (this->actionDuration > deltaTime) {
				INFO << "INCREASE THE VERTICAL VELOCITY" << std::endl;
				this->velocity.y += deltaTime * Player::jumpAcceleration;
				this->actionDuration -= deltaTime;
			} else {
				this->velocity.y += this->actionDuration * Player::jumpAcceleration;
				this->actionDuration = 0;
			}
		}

	} else {
		if (this->actionDuration > 0) {
			this->actionDuration = 0;
		}
	}

	/*if (this->jumping) {
		INFO << "We are jumping currently, time: " << this->actionDuration << std::endl;
	} else {
		INFO << "We are NOT jumping currently" << std::endl;
	}*/

	this->velocity.y -= deltaTime * Player::gravity;

	// Move backward
	//if (input.down){
		//this->worldLocation -= direction * deltaTime * Player::translationRate;
	//}
	// Strafe right
	if (input.right){
		if (this->velocity.x < 0) {
			this->velocity.x = 0;
		} else {
			this->velocity.x += Player::accelerationRate;
		}
	} else if (this->velocity.x > 0) {
		this->velocity.x = 0;
	}
	// Strafe left
	if (input.left){
		if (this->velocity.x > 0) {
			this->velocity.x = 0;
		} else {
			this->velocity.x -= Player::accelerationRate;
		}
	} else if (this->velocity.x < 0) {
		this->velocity.x = 0;
	}
}

// deltaTime is in seconds
void Player::Update(float deltaTime, std::vector<std::vector<char>> &worldArray) {
	this->HandleInputUpdate(deltaTime);
	this->HandleMovementUpdates(deltaTime, worldArray);
}

void Player::Render(void) {
	if (this->textureID != -1u) {
		this->setTexture(this->textureID);
	}

	glBindBuffer(GL_ARRAY_BUFFER, this->pointBufferID);
	glVertexAttribPointer(
		1,                  // attribute
		1,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*) 0 // array buffer offset
	);

	CHECKERRORS();

	this->getShaderManager().bindVector2("position", &this->position[0]);
	this->getShaderManager().setUniformFloat("textureGridDim", textureGridDim);

	if (this->velocity.x > 0) {
		this->facingForward = true;
	} else if (this->velocity.x < 0) {
		this->facingForward = false;
	}

	this->getShaderManager().setUniformBool("flipVertical", !facingForward);

	ShaderManager::getActiveShaderManager()->validateProgram();

	if (this->velocity.x != 0) {
		this->runNum = (this->runNum + 1) % 4;
	} else {
		this->runNum = 0;
	}

	glDrawArrays(GL_POINTS, this->runNum, 1);
}

void Player::Destroy(void) {
	if (this->textureID != -1u) {
		//this->
	}

	//GLuint textureID;
	//GLuint pointBufferID;
}

Player::~Player() {
}
