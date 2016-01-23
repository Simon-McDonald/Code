/*
 * HumanPlayer.h
 *
 *  Created on: Jan 10, 2016
 *      Author: Simon
 */

#ifndef SRC_HUMANPLAYER_H_
#define SRC_HUMANPLAYER_H_

#include "Board.h"
#include "Player.h"
#include "PlayerColour.h"

class HumanPlayer : public Player {
public:
	using Player::Player;

	bool performMove (Move*);
};

#endif /* SRC_HUMANPLAYER_H_ */
