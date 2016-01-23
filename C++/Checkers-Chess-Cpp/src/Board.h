/*
 * Board.h
 *
 *  Created on: Jan 2, 2016
 *      Author: Simon
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <list>

#include "BoardTypes.h"
#include "Piece.h"
#include "PlayerColour.h"

using namespace std;

class Board {
friend class Referee;
public:
	Board ();
	void setupCheckers ();
	void setupCheckers (PlayerColour (&arr)[numRows][numColumns]);

	void printBoard ();

	bool at (Piece*, RowIndex, ColumnIndex);
	bool at (Piece*, BoardLocation);

	~Board ();

private:
	list<Piece *> whitePlayerPieces;
	list<Piece *> blackPlayerPieces;

	Piece* board[8][8];

	Piece* get (RowIndex, ColumnIndex);
	Piece* get (BoardLocation);

	bool movePiece (Translation*);
	bool takePiece (BoardLocation*);
	bool addPiece (AddPiece*);

	void reset ();
	bool isGameOver ();
	PlayerColour getWinner ();
	void printBlankRow (PlayerColour);
};

#endif /* BOARD_H_ */
