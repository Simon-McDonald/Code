/*
 * Player.cpp
 *
 *  Created on: Jan 9, 2016
 *      Author: Simon
 */

#include "Player.h"

Player::Player (PlayerColour colour, Board *theBoard) : playerColour (colour) {
	this->theBoard = theBoard;
}

PlayerColour Player::getColour () {
	return this->playerColour;
}

Player::~Player () {}
