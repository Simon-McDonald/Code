/*
 * Board.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Simon
 */

#include <algorithm>
#include <iostream>

#include "Board.h"

using namespace std;

Board::Board () {
	this->reset ();
}

void Board::reset () {
	while (this->whitePlayerPieces.size() > 0) {
		delete this->whitePlayerPieces.back();
		this->whitePlayerPieces.pop_back ();
	}
	while (this->blackPlayerPieces.size() > 0) {
		delete this->blackPlayerPieces.back();
		this->blackPlayerPieces.pop_back ();
	}

	for (RowIndex rowIndex = RowIndex::zero; rowIndex < RowIndex::numRows; rowIndex = incRow(rowIndex)) {
		for (ColumnIndex colIndex = ColumnIndex::A; colIndex < ColumnIndex::numColumns; colIndex = incCol(colIndex)) {
			this->board[rowIndex][colIndex] = nullptr;
		}
	}
}

void Board::setupCheckers () {
	if (this->whitePlayerPieces.size() > 0 || this->blackPlayerPieces.size() > 0) {
		return;
	}

	for (RowIndex rowIndex = RowIndex::zero; rowIndex < RowIndex::three; rowIndex = RowIndex (rowIndex + 1)) {
		for (ColumnIndex colIndex = ColumnIndex (ColumnIndex::A + (rowIndex % 2)); colIndex < ColumnIndex::numColumns; colIndex = ColumnIndex (colIndex + 2)) {
			Piece *piece = new Piece ("checker", PlayerColour::white);
			this->whitePlayerPieces.push_back (piece);
			this->board [rowIndex][colIndex] = piece;
		}
	}

	for (RowIndex rowIndex = RowIndex::seven; rowIndex > RowIndex::four; rowIndex = RowIndex (rowIndex - 1)) {
		for (ColumnIndex colIndex = ColumnIndex (ColumnIndex::A + (rowIndex % 2)); colIndex < ColumnIndex::numColumns; colIndex = ColumnIndex (colIndex + 2)) {
			Piece *piece = new Piece ("checker", PlayerColour::black);
			this->blackPlayerPieces.push_back (piece);
			this->board [rowIndex][colIndex] = piece;
		}
	}
}

void Board::setupCheckers (PlayerColour (&initialArray)[numRows][numColumns]) {
	if (this->whitePlayerPieces.size() > 0 || this->blackPlayerPieces.size() > 0) {
		return;
	}

	for (RowIndex rowIndex = RowIndex::zero; rowIndex < RowIndex::numRows; rowIndex = RowIndex (rowIndex + 1)) {
		for (ColumnIndex colIndex = ColumnIndex::A; colIndex < ColumnIndex::numColumns; colIndex = ColumnIndex (colIndex + 1)) {
			if (initialArray[rowIndex][colIndex] == PlayerColour::white) {
				Piece *piece = new Piece ("checker", PlayerColour::white);
				this->whitePlayerPieces.push_back (piece);
				this->board [rowIndex][colIndex] = piece;
			} else if (initialArray[rowIndex][colIndex] == PlayerColour::black) {
				Piece *piece = new Piece ("checker", PlayerColour::black);
				this->blackPlayerPieces.push_back (piece);
				this->board [rowIndex][colIndex] = piece;
			}
		}
	}
}

bool Board::at (Piece *piece, RowIndex row, ColumnIndex col) {
	if (this->board[row][col] != nullptr) {
		if (piece != nullptr) {
			*piece = *this->board[row][col];
		}
		return true;
	}

	return false;
}

bool Board::at (Piece *piece, BoardLocation location) {
	if (this->board[location.row][location.col]) {
		if (piece != nullptr) {
			*piece = *this->board[location.row][location.col];
		}
		return true;
	}

	return false;
}

Piece* Board::get (RowIndex row, ColumnIndex col) {
	return this->board[row][col];
}

Piece* Board::get (BoardLocation location) {
	return this->board[location.row][location.col];
}

bool Board::movePiece (Translation* pieceMove) {
	bool pieceMoved = false;

	if (this->board[pieceMove->initialLocation.row][pieceMove->initialLocation.col] != nullptr &&
		this->board[pieceMove->finalLocation.row][pieceMove->finalLocation.col] == nullptr) {

		this->board[pieceMove->finalLocation.row][pieceMove->finalLocation.col] =
				this->board[pieceMove->initialLocation.row][pieceMove->initialLocation.col];
		this->board[pieceMove->initialLocation.row][pieceMove->initialLocation.col] = nullptr;

		pieceMoved = true;
	}

	if (!pieceMoved) {
		cout << "Could not move piece from (" << pieceMove->initialLocation.row << ", " << pieceMove->initialLocation.col << ") to ("
				<< pieceMove->finalLocation.row << ", " << pieceMove->finalLocation.col << ")" << endl;
	}

	return pieceMoved;
}

bool Board::takePiece (BoardLocation* pieceLocation) {
	bool removedPiece = false;
	Piece *piece = this->board[pieceLocation->row][pieceLocation->col];

	if (!removedPiece) {
		list<Piece *>::iterator pc_itr = find(this->whitePlayerPieces.begin(), this->whitePlayerPieces.end(), piece);

		if (pc_itr != this->whitePlayerPieces.end()) {
			this->whitePlayerPieces.remove(piece);
			removedPiece = true;
		}
	}

	if (!removedPiece) {
		list<Piece *>::iterator pc_itr = find(this->blackPlayerPieces.begin(), this->blackPlayerPieces.end(), piece);

		if (pc_itr != this->blackPlayerPieces.end()) {
			this->blackPlayerPieces.remove(piece);
			removedPiece = true;
		}
	}

	if (removedPiece) {
		this->board[pieceLocation->row][pieceLocation->col] = nullptr;
	} else {
		cout << "The piece specified at (" << pieceLocation->row << ", " << pieceLocation->col << ")" << "could not be removed from the players pieces!" << endl;
	}

	return removedPiece;
}

bool Board::addPiece (AddPiece* newPiece) {
	bool pieceAdded = false;

	if (this->board[newPiece->location.row][newPiece->location.col] == nullptr) {

		Piece *piece = new Piece (newPiece->piece);

		this->board[newPiece->location.row][newPiece->location.col] = piece;

		switch (piece->getColour()) {
		case PlayerColour::white:
			this->whitePlayerPieces.push_back(piece);
			pieceAdded = true;
			break;
		case PlayerColour::black:
			this->blackPlayerPieces.push_back(piece);
			pieceAdded = true;
			break;
		default:
			this->board[newPiece->location.row][newPiece->location.col] = nullptr;
		}
	}

	if (!pieceAdded) {
		cout << "Could not add piece at (" << newPiece->location.row << ", " << newPiece->location.col << ")" << endl;
	}

	return pieceAdded;
}

void Board::printBlankRow (PlayerColour firstSquareColour) {
	const string whiteSquare = "   ";
	const string blackSquare = "###";

	string combination;

	if (firstSquareColour == PlayerColour::white) {
		combination = whiteSquare + blackSquare;
	} else {
		combination = blackSquare + whiteSquare;
	}

	cout << "|";
	for (int index = 0; index < 4; index++) {
		cout << combination;
	}
	cout << "|";
}

void Board::printBoard () {
	cout << "  +------------------------+" << endl;

	for (RowIndex rowIndex = RowIndex::seven; rowIndex >= RowIndex::zero; rowIndex = RowIndex (rowIndex - 1)) {
		cout << "  ";
		printBlankRow ((rowIndex % 2 == 1) ? PlayerColour::black : PlayerColour::white);
		cout << endl;

		cout << rowIndex << " |";
		for (ColumnIndex colIndex = ColumnIndex::A; colIndex < ColumnIndex::numColumns; colIndex = ColumnIndex (colIndex + 1)) {
			PlayerColour currentSquareColour = ((rowIndex + colIndex) % 2 == 1) ?  PlayerColour::black : PlayerColour::white;

			cout << ((currentSquareColour == PlayerColour::white) ? ' ' : '#');

			if (this->board[rowIndex][colIndex] != nullptr) {
				cout << this->board[rowIndex][colIndex]->getSymbol();
			} else {
				cout << ((currentSquareColour == PlayerColour::white) ? ' ' : '#');
			}

			cout << ((currentSquareColour == PlayerColour::white) ? ' ' : '#');
		}
		cout << "|" << endl;

		cout << "  ";
		printBlankRow ((rowIndex % 2 == 1) ? PlayerColour::black : PlayerColour::white);
		cout << endl;
	}

	cout << "  +------------------------+" << endl;
	cout << "   ";
	for (ColumnIndex colIndex = ColumnIndex::A; colIndex < ColumnIndex::numColumns; colIndex = ColumnIndex (colIndex + 1)) {
		cout << " " << (char) (colIndex + 65) << " ";
	}
	cout << endl;
}

bool Board::isGameOver () {
	if (this->whitePlayerPieces.size() == 0 || this->blackPlayerPieces.size () == 0) {
		return true;
	}
	return false;
}

PlayerColour Board::getWinner () {
	if (this->whitePlayerPieces.size() == 0 && this->blackPlayerPieces.size() > 0) {
		return PlayerColour::black;
	} else if (this->whitePlayerPieces.size() > 0 && this->blackPlayerPieces.size() == 0) {
		return PlayerColour::white;
	}

	return PlayerColour::none;
}

Board::~Board () {}
