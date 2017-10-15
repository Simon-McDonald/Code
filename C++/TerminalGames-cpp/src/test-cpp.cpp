//============================================================================
// Name        : test-cpp.cpp
// Author      : Simon
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "MenuDisplay.h"
#include "SnakeGame.h"

int main ()
{
	Window::MenuDisplay mainMenu ("Main Menu");

	mainMenu.addMenuItem ("Snake", SnakeGame::RunSnakeGame);

	mainMenu.enterMenu ();

	return EXIT_SUCCESS;
} /* main */
