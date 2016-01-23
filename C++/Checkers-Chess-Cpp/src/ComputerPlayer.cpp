/*
 * ComputerPlayer.cpp
 *
 *  Created on: Jan 10, 2016
 *      Author: Simon
 */

#include <stdlib.h>
#include <iostream>

#include "ComputerPlayer.h"

using namespace std;

bool ComputerPlayer::performMove (Move *move) {

	bool requiredTake = false;
	vector<Move> moveOptions;
	vector<Move> moveRequired;

	for (RowIndex row = RowIndex::zero; row < RowIndex::numRows; row = RowIndex (row + 1)) {
		for (ColumnIndex col = ColumnIndex::A; col < ColumnIndex::numColumns; col = ColumnIndex (col + 1)) {
			Piece piece;

			if (theBoard->at(&piece, row, col) && piece.getColour() == this->getColour()) {

				if (generateRequiredMoves (&moveRequired, BoardLocation({row, col}), piece.isKing(), true)) {
					requiredTake = true;
					moveOptions.empty();
				}

				if (!requiredTake) {
					generateOptionalMoves (&moveOptions, {row, col}, piece.isKing());
				}
			}
		}
	}

	if (moveRequired.size() > 0) {
		cout << "Computer will take a piece!" << endl;
		int moveIndex = rand() % moveRequired.size();
		*move = moveRequired.at(moveIndex);
		return true;
	} else if (moveOptions.size() > 0) {
		cout << "Computer will move a piece!" << endl;
		int moveIndex = rand() % moveOptions.size();
		cout << "Number moves " << moveOptions.size() << ", take option " << moveIndex << "!" << endl;
		*move = moveOptions.at(moveIndex);
		return true;
	}
	cout << "Computer will quit!" << endl;
	return false;
}

bool ComputerPlayer::generateOptionalMoves(vector<Move>* moveList, BoardLocation initialLocation, bool isKing) {
	int movesAdded = 0;

	bool isForward = (this->getColour() == PlayerColour::white);

	bool pushColUp = ColumnIndex (initialLocation.col + 1) < ColumnIndex::numColumns;
	bool pushColDwn = ColumnIndex (initialLocation.col - 1) > ColumnIndex::minColumns;

	if ((isForward || isKing) && RowIndex (initialLocation.row + 1) < RowIndex::numRows) {
		if (pushColDwn && !this->theBoard->at(nullptr, RowIndex (initialLocation.row + 1), ColumnIndex (initialLocation.col - 1))) {
			Move move;
			move.movePieces.push_back ({initialLocation, {RowIndex (initialLocation.row + 1), ColumnIndex (initialLocation.col - 1)}});
			moveList->push_back(move);
			movesAdded++;
		}
		if (pushColUp && !this->theBoard->at(nullptr, RowIndex (initialLocation.row + 1), ColumnIndex (initialLocation.col + 1))) {
			Move move;
			move.movePieces.push_back ({initialLocation, {RowIndex (initialLocation.row + 1), ColumnIndex (initialLocation.col + 1)}});
			moveList->push_back(move);
			movesAdded++;
		}
	}
	if ((!isForward || isKing) && RowIndex (initialLocation.row - 1) > RowIndex::minRows) {
		if (pushColDwn && !this->theBoard->at(nullptr, RowIndex (initialLocation.row - 1), ColumnIndex (initialLocation.col - 1))) {
			Move move;
			move.movePieces.push_back ({initialLocation, {RowIndex (initialLocation.row - 1), ColumnIndex (initialLocation.col - 1)}});
			moveList->push_back(move);
			movesAdded++;
		}
		if (pushColUp && !this->theBoard->at(nullptr, RowIndex (initialLocation.row - 1), ColumnIndex (initialLocation.col + 1))) {
			Move move;
			move.movePieces.push_back ({initialLocation, {RowIndex (initialLocation.row - 1), ColumnIndex (initialLocation.col + 1)}});
			moveList->push_back(move);
			movesAdded++;
		}
	}

	return (movesAdded > 0);
}

/*bool ComputerPlayer::addOptionalMove (vector<Move>* moveList, BoardLocation initialLocation, RowIndex row, ColumnIndex col) {
	if (!this->theBoard->at(nullptr, row, col)) {
		Move move;
		move.movePieces.push_back ({initialLocation, {row, col}});
		moveList->push_back(move);
		return true;
	}
	return false;
}*/

bool ComputerPlayer::generateRequiredMoves(vector<Move>* moveList, BoardLocation initialLocation, bool isKing, bool isFirstCall) {

	Move currentMove;

	if (!isFirstCall) {
		currentMove = moveList->back();
		moveList->pop_back();
	}

	int movesAdded = 0;

	bool isForward = (this->getColour() == PlayerColour::white);

	bool pushColUp = initialLocation.col + 2 < ColumnIndex::numColumns;
	bool pushColDwn = initialLocation.col - 2 > ColumnIndex::minColumns;
	bool pushRowUp = initialLocation.row + 2 < RowIndex::numRows;
	bool pushRowDwn = initialLocation.row - 2 > RowIndex::minRows;

	// Can jump two positions up and to the right
	if ((isForward || isKing) && pushRowUp && pushColUp) {
		Piece tempPiece;
		// Check destination is empty and piece in between is opposite colour
		if (!this->theBoard->at(nullptr, incRow(incRow(initialLocation.row)), incCol(incCol(initialLocation.col))) &&
			this->theBoard->at(&tempPiece, incRow(initialLocation.row), incCol(initialLocation.col)) &&
			tempPiece.getColour() != this->getColour()) {

			Move tempMove = currentMove;
			tempMove.movePieces.push_back({initialLocation, {incRow(incRow(initialLocation.row)), incCol(incCol(initialLocation.col))}});
			moveList->push_back(tempMove);
			movesAdded++;
			this->generateRequiredMoves (moveList, {incRow(incRow(initialLocation.row)), incCol(incCol(initialLocation.col))}, isKing, false);
		}
	}

	// Can jump two positions up and to the left
	if ((isForward || isKing) && pushRowUp && pushColDwn) {
		Piece tempPiece;
		// Check destination is empty and piece in between is opposite colour
		if (!this->theBoard->at(nullptr, incRow(incRow(initialLocation.row)), decCol(decCol(initialLocation.col))) &&
			this->theBoard->at(&tempPiece, incRow(initialLocation.row), decCol(initialLocation.col)) &&
			tempPiece.getColour() != this->getColour()) {

			Move tempMove = currentMove;
			tempMove.movePieces.push_back({initialLocation, {incRow(incRow(initialLocation.row)), decCol(decCol(initialLocation.col))}});
			moveList->push_back(tempMove);
			movesAdded++;
			this->generateRequiredMoves (moveList, {incRow(incRow(initialLocation.row)), decCol(decCol(initialLocation.col))}, isKing, false);
		}
	}

	// Can jump two positions up and to the right
	if ((!isForward || isKing) && pushRowDwn && pushColUp) {
		Piece tempPiece;
		// Check destination is empty and piece in between is opposite colour
		if (!this->theBoard->at(nullptr, decRow(decRow(initialLocation.row)), incCol(incCol(initialLocation.col))) &&
			this->theBoard->at(&tempPiece, decRow(initialLocation.row), incCol(initialLocation.col)) &&
			tempPiece.getColour() != this->getColour()) {

			Move tempMove = currentMove;
			tempMove.movePieces.push_back({initialLocation, {decRow(decRow(initialLocation.row)), incCol(incCol(initialLocation.col))}});
			moveList->push_back(tempMove);
			movesAdded++;
			this->generateRequiredMoves (moveList, {decRow(decRow(initialLocation.row)), incCol(incCol(initialLocation.col))}, isKing, false);
		}
	}

	// Can jump two positions up and to the left
	if ((!isForward || isKing) && pushRowDwn && pushColDwn) {
		Piece tempPiece;
		// Check destination is empty and piece in between is opposite colour
		if (!this->theBoard->at(nullptr, decRow(decRow(initialLocation.row)), decCol(decCol(initialLocation.col))) &&
			this->theBoard->at(&tempPiece, decRow(initialLocation.row), decCol(initialLocation.col)) &&
			tempPiece.getColour() != this->getColour()) {

			Move tempMove = currentMove;
			tempMove.movePieces.push_back({initialLocation, {decRow(decRow(initialLocation.row)), decCol(decCol(initialLocation.col))}});
			moveList->push_back(tempMove);
			movesAdded++;
			this->generateRequiredMoves (moveList, {decRow(decRow(initialLocation.row)), decCol(decCol(initialLocation.col))}, isKing, false);
		}
	}

	if (!isFirstCall && movesAdded == 0) {
		moveList->push_back(currentMove);
	}

	return (movesAdded > 0);
}
