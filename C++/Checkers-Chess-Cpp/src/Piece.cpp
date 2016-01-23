/*
 * Piece.cpp
 *
 *  Created on: Jan 10, 2016
 *      Author: Simon
 */

#include "Piece.h"

Piece::Piece () : colour (PlayerColour::none), kingPiece (false) {}
Piece::Piece (string type, PlayerColour colour) : colour (colour), kingPiece (false) {}

string Piece::getType () {
	return this->type;
}

PlayerColour Piece::getColour () {
	return this->colour;
}

char Piece::getSymbol () {
	if (this->colour == PlayerColour::white) {
		if (!this->kingPiece) {
			return 'o';
		} else {
			return 'O';
		}
	} else {
		if (!this->kingPiece) {
			return 'x';
		} else {
			return 'X';
		}
	}
}

void Piece::kingMe () {
	this->kingPiece = true;
}

bool Piece::isKing () {
	return this->kingPiece;
}

Piece::~Piece () {}
