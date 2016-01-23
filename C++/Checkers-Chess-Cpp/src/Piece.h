/*
 * Piece.h
 *
 *  Created on: Jan 2, 2016
 *      Author: Simon
 */

#ifndef PIECE_H_
#define PIECE_H_

#include <string>

#include "PlayerColour.h"

using namespace std;

class Piece {
public:
	Piece();
	Piece(string, PlayerColour);

	string getType ();
	PlayerColour getColour ();
	char getSymbol ();

	void kingMe ();
	bool isKing ();

	~Piece();
protected:
	string type;
	PlayerColour colour;
	bool kingPiece;
};

#endif /* PIECE_H_ */
