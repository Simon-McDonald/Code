/*
 * HumanPlayer.cpp
 *
 *  Created on: Jan 10, 2016
 *      Author: Simon
 */

#include <iostream>
#include <string>

#include "HumanPlayer.h"

using namespace std;

bool HumanPlayer::performMove (Move* move) {
	bool isValidInput = false;

	while (!isValidInput) {
		string line;

		cout << "Enter move: ";
		getline (cin, line);

		BoardLocation prevLocation, currentLocation;
		bool isFirst = true;
		bool rowFound = false;
		bool colFound = false;

		if (line.length() == 0) {
			break;
		}

		for (unsigned int str_idx = 0; str_idx < line.length(); str_idx++) {
			if (charToRowIndex(line.at(str_idx)) != RowIndex::numRows) {
				currentLocation.row = charToRowIndex(line.at(str_idx));
				rowFound = true;
			} else if (charToColumnIndex(line.at(str_idx)) != ColumnIndex::numColumns) {
				currentLocation.col = charToColumnIndex(line.at(str_idx));
				colFound = true;
			} else {
				rowFound = false;
				colFound = false;
			}

			if (colFound && rowFound) {
				if (isFirst) {
					isFirst = false;
				} else {
					move->movePieces.push_back({prevLocation, currentLocation});
				}
				prevLocation = currentLocation;
				rowFound = false;
				colFound = false;
			}
		}

		isValidInput = (move->movePieces.size() > 0);
	}

	return isValidInput;
}
