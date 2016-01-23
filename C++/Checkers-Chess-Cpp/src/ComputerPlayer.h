/*
 * ComputerPlayer.h
 *
 *  Created on: Jan 10, 2016
 *      Author: Simon
 */

#ifndef SRC_COMPUTERPLAYER_H_
#define SRC_COMPUTERPLAYER_H_

#include <vector>

#include "Board.h"
#include "Player.h"
#include "PlayerColour.h"

using namespace std;

class ComputerPlayer : public Player {
public:
	using Player::Player;

	bool performMove (Move*);

private:
	bool generateOptionalMoves(vector<Move>*, BoardLocation, bool);
	//bool addOptionalMove (vector<Move>*, BoardLocation, RowIndex, ColumnIndex);
	bool generateRequiredMoves(vector<Move>*, BoardLocation, bool, bool);
	bool recursiveJumpGenerator(vector<Move>*, bool);
};

#endif /* SRC_COMPUTERPLAYER_H_ */
