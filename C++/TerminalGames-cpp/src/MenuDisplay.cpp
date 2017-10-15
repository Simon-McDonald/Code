/*
 * MenuDisplay.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: Simon
 */

#include <iostream>

#include "MenuDisplay.h"

Window::MenuDisplay::MenuDisplay (std::string title) : title (title), currentIndex (0) {
	this->menuOptions.push_back({"Quit", nullptr});
} /* Window::MenuDisplay::MenuDisplay */

Window::MenuDisplay::~MenuDisplay (void) {} /* Window::MenuDisplay::~MenuDisplay */

void Window::MenuDisplay::addMenuItem (std::string menuString, void (*menuItemCb) (void)) {
	this->menuOptions.insert(this->menuOptions.begin() + (this->menuOptions.size () - 1), {menuString, menuItemCb});
} /* Window::MenuDisplay::addMenuItem */

void Window::MenuDisplay::printMenu (void) {
	Window::ConsoleWindow* display = Window::ConsoleWindow::getWindow();

	int currentRow = Window::ConsoleWindow::getWindow()->getHeight () - 3;
	display->setTitleString (currentRow, this->title.c_str (), this->title.size ());
	currentRow -= 4;

	display->addBorder (1, currentRow, display->getWidth () - 2, 1, 3);

	currentRow -= 2;

	for (unsigned int menuIdx = 0; menuIdx < this->menuOptions.size(); menuIdx++) {
		display->setString(8,
						   currentRow,
				           this->menuOptions.at(menuIdx).menuString.c_str(),
						   this->menuOptions.at(menuIdx).menuString.size());
		currentRow -= 2;
	}
} /* Window::MenuDisplay::printMenu */

void Window::MenuDisplay::updateMenu (void) {
	Window::ConsoleWindow* display = Window::ConsoleWindow::getWindow();
	int currentRow = display->getHeight () - 3;
	currentRow -= 6;

	for (unsigned int menuIdx = 0; menuIdx < this->menuOptions.size(); menuIdx++) {
		display->setString(3, currentRow, (menuIdx == this->currentIndex) ? "->" : " *", 2);
		currentRow -= 2;
	}
} /* Window::MenuDisplay::updateMenu */

void Window::MenuDisplay::enterMenu (void) {
	Window::ConsoleWindow *display = Window::ConsoleWindow::getWindow();

	bool userHasQuit = false;
	display->clearScreen ();
	this->printMenu ();

	while (!userHasQuit) {
		this->updateMenu ();
		display->update ();

		inputStruct input = display->getUserInput (true);

		if (input.keyPress) {
			switch (input.key) {
			case UserInput::UP:
				if (this->currentIndex > 0) {
					this->currentIndex--;
				}
				break;
			case UserInput::DOWN:
				if (this->currentIndex < this->menuOptions.size () - 1) {
					this->currentIndex++;
				}
				break;
			case UserInput::ENTER:
				if (this->menuOptions.at(this->currentIndex).menuItemCb) {
					this->menuOptions.at(this->currentIndex).menuItemCb ();
					this->printMenu ();
					display->clearScreen ();
					this->printMenu ();
				} else {
					userHasQuit = true;
				}
				break;
			default:
				break;
			}
		}
	}
} /* Window::MenuDisplay::enterMenu */
