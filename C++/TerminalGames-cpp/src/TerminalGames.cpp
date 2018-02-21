//============================================================================
// Name        : TerminalGames.cpp
// Author      : Simon
// Version     :
// Copyright   : Your copyright notice
// Description : Run application
//============================================================================

#include "MenuDisplay.h"
#include "SnakeGame.h"

int main() {
    MenuDisplay mainMenu("Main Menu");
    mainMenu.addMenuItem("Snake", SnakeGame::RunSnakeGame);

    mainMenu.enterMenu();

    return EXIT_SUCCESS;
}
