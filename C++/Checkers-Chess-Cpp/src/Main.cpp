/*
 * Main.cpp
 *
 *  Created on: Jan 2, 2016
 *      Author: Simon
 */

#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "Board.h"
#include "ComputerPlayer.h"
#include "HumanPlayer.h"
#include "Player.h"
#include "PlayerColour.h"
#include "Referee.h"

using namespace std;

/* BOARD is flipped on the horizontal axis */
PlayerColour testBoard[numRows][numColumns] = {
		{white, none,  white, none,  white, none,  white, none},
		{none,  white, none,  white, none,  white, none,  white},
		{white, none,  white, none,  white, none,  white, none},
		{none,  none,  none,  none, none,  none,  none,  none},
		{none,  none,  none,  none,  none,  none,  none,  none},
		{none,  none,  none,  black,  none,  black,  none,  black},
		{white, none,  black, none,  none, none,  white,  none},
		{none,  none,  none,  none,  none,  none,  none,  none}
};

int main (int argv, char **argc) {

	PlayerColour humanPlayerColour = PlayerColour::white;

	//string userEntry;
	//cout << "Which colour to play as, (w)hite/(b)lack" << endl;
	//cin >> userEntry;
	//Set playerGoesFirst

	cout << "Human player is " << playerColourString (humanPlayerColour) << endl;
	cout << "Computer player is " << playerColourString (flipPlayerColour (humanPlayerColour)) << endl;

	Board theBoard;

	theBoard.setupCheckers(testBoard);

	Player *whitePlayer = (humanPlayerColour == PlayerColour::white) ?
			(Player *) (new HumanPlayer (PlayerColour::white, &theBoard)) :
			(Player *) (new ComputerPlayer (PlayerColour::white, &theBoard));
	Player *blackPlayer = (humanPlayerColour == PlayerColour::black) ?
			(Player *) (new HumanPlayer (PlayerColour::black, &theBoard)) :
			(Player *) (new ComputerPlayer (PlayerColour::black, &theBoard));

	Referee referee(&theBoard, whitePlayer, blackPlayer);

	bool continuePlaying = true;
	PlayerColour currentPlayerColour = PlayerColour::white;

	theBoard.printBoard();

	while (continuePlaying) {
		continuePlaying = referee.performPlayerMove(currentPlayerColour);

		if (continuePlaying) {
			theBoard.printBoard ();
			currentPlayerColour = flipPlayerColour (currentPlayerColour);

			continuePlaying = !referee.isGameOver();
		}
	}

	if (referee.isGameOver()) {
		PlayerColour winningPlayerColour = referee.getWinnerColour();

		if (winningPlayerColour == humanPlayerColour) {
			cout << "The Human player won!!!" << endl;
		} else {
			cout << "The Computer player won!!!" << endl;
		}
	} else {
		if (currentPlayerColour == humanPlayerColour) {
			cout << "The Human player quit the game!!!" << endl;
		} else {
			cout << "The Computer player quit the game!!!" << endl;
		}
	}

	delete whitePlayer;
	delete blackPlayer;

	return EXIT_SUCCESS;
}
