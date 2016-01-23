/*
 * BoardTypes.h
 *
 *  Created on: Jan 10, 2016
 *      Author: Simon
 */

#ifndef BOARDTYPES_H_
#define BOARDTYPES_H_

#include <iostream>
#include <list>
#include <string>

#include "Piece.h"

using namespace std;

enum RowIndex {
	minRows = -1, zero = 0, one, two, three, four, five, six, seven, numRows
};

enum ColumnIndex {
	minColumns = -1, A = 0, B, C, D, E, F, G, H, numColumns
};

#define incRow(row) RowIndex(row + 1)
#define decRow(row) RowIndex(row - 1)
#define incCol(col) ColumnIndex(col + 1)
#define decCol(col) ColumnIndex(col - 1)

inline RowIndex charToRowIndex (char row) {
	RowIndex rowIndex = RowIndex::numRows;

	switch (toupper (row)) {
	case '0':
		rowIndex = RowIndex::zero;
		break;
	case '1':
		rowIndex = RowIndex::one;
		break;
	case '2':
		rowIndex = RowIndex::two;
		break;
	case '3':
		rowIndex = RowIndex::three;
		break;
	case '4':
		rowIndex = RowIndex::four;
		break;
	case '5':
		rowIndex = RowIndex::five;
		break;
	case '6':
		rowIndex = RowIndex::six;
		break;
	case '7':
		rowIndex = RowIndex::seven;
		break;
	default:
		rowIndex = RowIndex::numRows;
		break;
	}
	return rowIndex;
}

inline char rowIndexToChar (RowIndex rowIndex) {
	switch (rowIndex) {
	case RowIndex::zero:
		return '0';
	case RowIndex::one:
		return '1';
	case RowIndex::two:
		return '2';
	case RowIndex::three:
		return '3';
	case RowIndex::four:
		return '4';
	case RowIndex::five:
		return '5';
	case RowIndex::six:
		return '6';
	case RowIndex::seven:
		return '7';
	default:
		return ' ';
	}
}

inline ColumnIndex charToColumnIndex (char col) {
	switch (toupper (col)) {
	case 'A':
		return ColumnIndex::A;
	case 'B':
		return ColumnIndex::B;
	case 'C':
		return ColumnIndex::C;
	case 'D':
		return ColumnIndex::D;
	case 'E':
		return ColumnIndex::E;
	case 'F':
		return ColumnIndex::F;
	case 'G':
		return ColumnIndex::G;
	case 'H':
		return ColumnIndex::H;
	default:
		return ColumnIndex::numColumns;
	}
}

inline char columnIndexToChar (ColumnIndex colIndex) {
	switch (colIndex) {
	case ColumnIndex::A:
		return 'A';
	case ColumnIndex::B:
		return 'B';
	case ColumnIndex::C:
		return 'C';
	case ColumnIndex::D:
		return 'D';
	case ColumnIndex::E:
		return 'E';
	case ColumnIndex::F:
		return 'F';
	case ColumnIndex::G:
		return 'G';
	case ColumnIndex::H:
		return 'H';
	default:
		return ' ';
	}
}

typedef struct {
	RowIndex row;
	ColumnIndex col;
} BoardLocation;

inline bool operator== (BoardLocation loc1, BoardLocation loc2) {
	return (loc1.row == loc2.row && loc1.col == loc2.col);
}
inline bool operator!= (BoardLocation loc1, BoardLocation loc2) {
	return !(loc1 == loc2);
}

typedef struct {
	BoardLocation initialLocation;
	BoardLocation finalLocation;
} Translation;

typedef struct {
	BoardLocation location;
	Piece piece;
} AddPiece;

typedef struct {
	list<Translation> movePieces;
	list<BoardLocation> takenPieces;
	list<AddPiece> newPieces;
} Move;

inline void AddPieceMove (Move *move, BoardLocation initialLocation, BoardLocation finalLocation) {
	move->movePieces.push_back({initialLocation, initialLocation});
}

inline void AddTakePiece (Move *move, BoardLocation takeLocation) {
	move->takenPieces.push_back(takeLocation);
}

inline void AddNewPiece (Move *move, BoardLocation newLocation, Piece newPiece) {
	move->newPieces.push_back({newLocation, newPiece});
}

inline string printBoardLocation (BoardLocation location) {
	return string({columnIndexToChar (location.col), rowIndexToChar (location.row)});
}

inline string printTranslation (Translation translation) {
	return printBoardLocation (translation.initialLocation) + "->" + printBoardLocation (translation.finalLocation);
}

inline string printMove (Move *move) {
	string moveString = "";
	bool stringStart = true;

	if (move->movePieces.size() > 0) {
		if (!stringStart) {
			moveString += ", ";
		} else {
			stringStart = false;
		}

		moveString.append("Translation:");

		for (list<Translation>::iterator mp_itr = move->movePieces.begin();
			 mp_itr != move->movePieces.end();
			 mp_itr++) {

			moveString += ' ';
			moveString += columnIndexToChar(mp_itr->initialLocation.col);
			moveString += rowIndexToChar(mp_itr->initialLocation.row);
			moveString += "->";
			moveString += columnIndexToChar(mp_itr->finalLocation.col);
			moveString += rowIndexToChar(mp_itr->finalLocation.row);
		}
	}

	if (move->takenPieces.size() > 0) {
		if (!stringStart) {
			moveString += ", ";
		} else {
			stringStart = false;
		}

		moveString.append("TakenPieces:");

		for (list<BoardLocation>::iterator tp_itr = move->takenPieces.begin();
			 tp_itr != move->takenPieces.end();
			 tp_itr++) {

			moveString += ' ';
			moveString += columnIndexToChar(tp_itr->col);
			moveString += rowIndexToChar(tp_itr->row);
		}
	}

	if (move->newPieces.size() > 0) {
		if (!stringStart) {
			moveString += ", ";
		} else {
			stringStart = false;
		}

		moveString.append("NewPieces:");

		for (list<AddPiece>::iterator np_itr = move->newPieces.begin();
			 np_itr != move->newPieces.end();
			 np_itr++) {

			moveString += ' ';
			moveString += columnIndexToChar(np_itr->location.col);
			moveString += rowIndexToChar(np_itr->location.row);
		}
	}

	return moveString;
}

#endif /* BOARDTYPES_H_ */
