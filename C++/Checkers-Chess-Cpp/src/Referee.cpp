/*
 * Referee.cpp
 *
 *  Created on: Jan 15, 2016
 *      Author: Simon
 */

#include <algorithm>
#include <iostream>
#include <stdlib.h>

#include "Referee.h"

using namespace std;

Referee::Referee (Board *theBoard, Player *whitePlayer, Player *blackPlayer) : theBoard (theBoard), whitePlayer (whitePlayer), blackPlayer (blackPlayer) {}

bool Referee::performPlayerMove (PlayerColour currentPlayerColour) {
	bool moveSuccess = false;
	Player *currentPlayer = nullptr;

	if (currentPlayerColour == PlayerColour::white) {
		currentPlayer = this->whitePlayer;
	} else if (currentPlayerColour == PlayerColour::black) {
		currentPlayer = this->blackPlayer;
	} else {
		return false;
	}

	int tries = 0;

	while (tries < 3 && !moveSuccess) {
		Move playerMove;
		bool moveStatus = currentPlayer->performMove(&playerMove);

		if (moveStatus) {
			if (this->validateMove(currentPlayerColour, &playerMove)) {
				moveSuccess = this->performMove(&playerMove);
			}
		}
	}
	return moveSuccess;
}

bool Referee::validateMove (PlayerColour currentPlayerColour, Move* playerMove) {

	cout << "The " << playerColourString (currentPlayerColour) << " player submitted the move: " << printMove(playerMove) << endl;

	bool isValid = true;
	Move moveCopy = *playerMove;

	if (playerMove->movePieces.size() == 0) {
		cout << "No moves were provided by the " << playerColourString(currentPlayerColour) << " player!" << endl;
		isValid = false;
	}

	BoardLocation previousLocation;

	bool isKing;
	bool isForward = (currentPlayerColour == PlayerColour::white);
	bool isJump;
	bool isFirst = true;

	for (list<Translation>::iterator mp_itr = moveCopy.movePieces.begin();
		 mp_itr != moveCopy.movePieces.end() && isValid;
		 mp_itr++) {

		if (((isFirst && this->theBoard->get(mp_itr->initialLocation) != nullptr &&
			this->theBoard->get(mp_itr->initialLocation)->getColour() == currentPlayerColour) ||
			(!isFirst && isJump && this->theBoard->get(mp_itr->initialLocation) == nullptr)) &&
			this->theBoard->get(mp_itr->finalLocation) == nullptr) {

			if (!isFirst) {
				if (previousLocation != mp_itr->initialLocation) {
					cout << "Move chain did not match between translations!" << endl;
					isValid = false;
					break;
				}
			}

			isKing = (isFirst) ? this->theBoard->get(mp_itr->initialLocation)->isKing() : isKing;

			int deltaRow = mp_itr->finalLocation.row - mp_itr->initialLocation.row;
			int deltaCol = mp_itr->finalLocation.col - mp_itr->initialLocation.col;

			// Check directionality
			if (!isKing && (deltaRow < 0 && isForward) && (deltaRow > 0 && !isForward)) {
				cout << "Move did not translate in correct direction!" << endl;
				isValid = false;
				break;
			}

			int absDeltaRow = abs(deltaRow);
			int absDeltaCol = abs(deltaCol);

			// Check that is diagonal and one or two jumps
			if (absDeltaRow != absDeltaCol || absDeltaRow > 2) {
				cout << "Move traversed a too greater distance!" << endl;
				isValid = false;
				break;
			}

			if (isFirst) {
				isJump = (absDeltaRow == 2);
			} else if (absDeltaRow != 2) {
				cout << "The first move was a jump but subsequent move was not two squares!" << endl;
				isValid = false;
				break;
			}

			if (isJump) {
				BoardLocation jumpedLoc = {RowIndex (mp_itr->initialLocation.row + deltaRow/2),
										   ColumnIndex (mp_itr->initialLocation.col + deltaCol/2)};

				if (this->theBoard->get(jumpedLoc) != nullptr &&
					this->theBoard->get(jumpedLoc)->getColour() != flipPlayerColour(currentPlayerColour)) {
					isValid = false;
				}
			}

			previousLocation = mp_itr->finalLocation;
			isFirst = false;
		} else {
			cout << "Initial location or final destination were not valid!" << endl;
			isValid = false;
		}
	}

	// must check that further takes are not required
	if (isJump && isValid &&
		(previousLocation.row != RowIndex::zero && currentPlayerColour == PlayerColour::black) &&
		(previousLocation.row != RowIndex::seven && currentPlayerColour == PlayerColour::white)) {

		cout << "There was a jump, check that there should not be more jumps!" << endl;

		vector<BoardLocation> pieceTakes;

		if (this->generateJumpList(&pieceTakes, previousLocation, currentPlayerColour, isKing)) {

			unsigned int numHits = 0;

			for (vector<BoardLocation>::iterator pt_itr = pieceTakes.begin();
				 pt_itr != pieceTakes.end();
				 pt_itr++) {

				bool notFound = true;

				for (list<BoardLocation>::iterator mtp_itr = moveCopy.takenPieces.begin();
					 mtp_itr != moveCopy.takenPieces.end();
					 mtp_itr++) {
					if (pt_itr->col == mtp_itr->col && pt_itr->row == mtp_itr->row) {
						numHits++;
						notFound = false;
						break;
					}
				}

				if (notFound) {
					break;
				}
			}

			if (numHits != pieceTakes.size()) {
				cout << "Player could have taken more pieces in move!" << endl;
				isValid = false;
			}
		}

	} else if (!isJump) {

		for (RowIndex row = RowIndex::zero; row < RowIndex::numRows; row = RowIndex (row + 1)) {
			for (ColumnIndex col = ColumnIndex::A; col < ColumnIndex::numColumns; col = ColumnIndex (col + 1)) {

				if (this->theBoard->get(row, col) != nullptr &&
					this->theBoard->get(row, col)->getColour() == currentPlayerColour) {

					if (this->generateJumpList(nullptr, {row, col}, currentPlayerColour,
							this->theBoard->get(row, col)->isKing())) {
						cout << "Move did not take piece but there was a piece available to take!" << endl;
						row = RowIndex::numRows;
						col = ColumnIndex::numColumns;
						isValid = false;
					}
				}
			}
		}
	}

	if (isValid) {
		cout << "The " << playerColourString (currentPlayerColour) << " players move "
				<< "is valid." << endl;
	} else {
		cout << "The " << playerColourString (currentPlayerColour) << " players move "
						<< "is invalid!!!" << endl;
	}

	return isValid;
}

bool Referee::generateJumpList (vector<BoardLocation>* moveList, BoardLocation pieceLocation,
								PlayerColour currentColour, bool isKing) {

	vector<BoardLocation> potentialJumps;

	bool isForward = (currentColour == PlayerColour::white);
	bool pushColUp = ColumnIndex (pieceLocation.col + 2) < ColumnIndex::numColumns;
	bool pushColDwn = ColumnIndex (pieceLocation.col - 2) > ColumnIndex::minColumns;

	if ((isForward || isKing) && RowIndex (pieceLocation.row + 2) < RowIndex::numRows) {
		if (pushColUp) {
			potentialJumps.push_back({RowIndex (pieceLocation.row + 2), ColumnIndex (pieceLocation.col + 2)});
		}
		if (pushColDwn) {
			potentialJumps.push_back({RowIndex (pieceLocation.row + 2), ColumnIndex (pieceLocation.col - 2)});
		}
	}
	if ((!isForward || isKing) && RowIndex (pieceLocation.row - 2) > RowIndex::minRows) {
		if (pushColUp) {
			potentialJumps.push_back({RowIndex (pieceLocation.row - 2), ColumnIndex (pieceLocation.col + 2)});
		}
		if (pushColDwn) {
			potentialJumps.push_back({RowIndex (pieceLocation.row - 2), ColumnIndex (pieceLocation.col - 2)});
		}
	}

	int numTaken = 0;

	for (vector<BoardLocation>::iterator pj_itr = potentialJumps.begin();
		 pj_itr != potentialJumps.end();
		 pj_itr++) {

		BoardLocation jumpLoc = *pj_itr;

		if (this->theBoard->get(jumpLoc) == nullptr) {
			BoardLocation location = {RowIndex ((jumpLoc.row + pieceLocation.row)/2),
					                  ColumnIndex ((jumpLoc.col + pieceLocation.col)/2)};

			if (this->theBoard->get(location) != nullptr &&
				this->theBoard->get(location)->getColour() != currentColour) {
				if (moveList != nullptr) {
					moveList->push_back (location);
				}
				numTaken++;
			}
		}
	}

	return (numTaken > 0);
}

bool Referee::performMove (Move* playerMove) {
	cout << "Perform the move: " << printMove(playerMove) << endl;

	bool moveSuccess = true;

	bool isFirst;
	PlayerColour currentColour;
	BoardLocation finalLocation;

	for (list<Translation>::iterator mp_itr = playerMove->movePieces.begin();
		 mp_itr != playerMove->movePieces.end();
		 mp_itr++) {

		if (isFirst) {
			currentColour = this->theBoard->get(mp_itr->initialLocation)->getColour();
			isFirst = false;
		}

		//cout << "Move piece: " << printTranslation (*mp_itr) << endl;
		moveSuccess = moveSuccess && theBoard->movePiece(&(*mp_itr));

		if (abs(mp_itr->initialLocation.row - mp_itr->finalLocation.row) == 2) {
		// Move takes an opposing piece

			int deltaRow = mp_itr->finalLocation.row - mp_itr->initialLocation.row;
			int deltaCol = mp_itr->finalLocation.col - mp_itr->initialLocation.col;

			BoardLocation jumpedLoc = {RowIndex (mp_itr->initialLocation.row + deltaRow/2),
									   ColumnIndex (mp_itr->initialLocation.col + deltaCol/2)};

			moveSuccess = moveSuccess && theBoard->takePiece(&jumpedLoc);

			//cout << "Remove piece at: " << printBoardLocation (jumpedLoc) << endl;
		}
		finalLocation = mp_itr->finalLocation;
	}

	if ((finalLocation.row == RowIndex::zero && currentColour == PlayerColour::black) ||
		(finalLocation.row == RowIndex::seven && currentColour == PlayerColour::white)) {

		this->theBoard->get(finalLocation)->kingMe();
	}

	if (moveSuccess) {
		cout << "Completed " << endl;
	} else {
		cout << "Failed to perform the submitted player move!" << endl;
	}

	return moveSuccess;
}

bool Referee::isGameOver () {
	return theBoard->isGameOver();
}

PlayerColour Referee::getWinnerColour () {
	return theBoard->getWinner();
}

Referee::~Referee () {}
