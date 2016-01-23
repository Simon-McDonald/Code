/*
 * Referee.h
 *
 *  Created on: Jan 15, 2016
 *      Author: Simon
 */

#ifndef REFEREE_H_
#define REFEREE_H_

#include <vector>

#include "Board.h"
#include "Player.h"
#include "PlayerColour.h"

using namespace std;

class Referee {
public:
	Referee (Board*, Player*, Player*);

	bool performPlayerMove (PlayerColour);
	bool isGameOver ();
	PlayerColour getWinnerColour ();

	~Referee ();
private:
	Board *theBoard;
	Player *whitePlayer;
	Player *blackPlayer;

	bool validateMove (PlayerColour, Move*);
	bool performMove (Move*);
	bool generateJumpList (vector<BoardLocation>*, BoardLocation, PlayerColour, bool);
};

#endif /* REFEREE_H_ */
