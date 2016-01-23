/*
 * Player.h
 *
 *  Created on: Jan 9, 2016
 *      Author: Simon
 */

#ifndef SRC_PLAYER_H_
#define SRC_PLAYER_H_

#include "Board.h"
#include "BoardTypes.h"
#include "PlayerColour.h"

class Player {
public:
	Player (PlayerColour, Board*);

	PlayerColour getColour ();

	virtual bool performMove (Move*) = 0;

	virtual ~Player ();
protected:
	PlayerColour playerColour;
	Board *theBoard;
};

#endif /* SRC_PLAYER_H_ */
