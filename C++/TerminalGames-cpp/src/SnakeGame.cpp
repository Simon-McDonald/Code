/*
 * SnakeGame.cpp
 *
 *  Created on: Mar 8, 2016
 *      Author: Simon
 */

#include <iostream>
#include <stdio.h>

#include "MenuDisplay.h"
#include "SnakeGame.h"

/*const int SnakeGame::maxLength = 32;
 const int SnakeGame::startUpdateDuration_ms = 200;
 const int SnakeGame::deltaUpdateDuration_ms = 5;
 const int SnakeGame::startLength = 5;*/

const int SnakeGame::maxLength;
const int SnakeGame::startUpdateDuration_ms;
const int SnakeGame::deltaUpdateDuration_ms;
const int SnakeGame::startLength;

SnakeGame::SnakeGame(char x, char y) :
    score(0), targetX(-1), targetY(-1), dirX(1), dirY(0), headIndex(SnakeGame::startLength - 1) {

    memset(snakeCoordX, -1, SnakeGame::maxLength);
    memset(snakeCoordY, -1, SnakeGame::maxLength);

    for (int posIndex = 0; posIndex < SnakeGame::startLength; posIndex++) {
        snakeCoordX[posIndex] = x;
        snakeCoordY[posIndex] = y;
    }
}

SnakeGame::~SnakeGame() {
}

void SnakeGame::RunSnakeGame() {
    MenuDisplay snakeMenu("Snake");
    snakeMenu.addMenuItem("Start", SnakeGame::start);
    snakeMenu.enterMenu();
}

void SnakeGame::start() {
    ConsoleWindow *display = ConsoleWindow::getWindow();
    SnakeGame game(display->getWidth() / 2, display->getHeight() / 2);
    display->clearScreen();

    bool playerHasQuit = false;
    bool playerHasDied = false;

    game.setTarget();
    game.setDisplay();

    while (!playerHasQuit && !playerHasDied) {
        game.updateDisplay();
        display->update();

        playerHasQuit = game.getUserInput();

        playerHasDied = game.moveSnake();
    }
}

void SnakeGame::setDisplay() {
    ConsoleWindow *display = ConsoleWindow::getWindow();
    display->clearScreen();
}

void SnakeGame::updateDisplay() {
    ConsoleWindow *display = ConsoleWindow::getWindow();

    if ((this->targetX != -1) && (this->targetY != -1)) {
        display->setElem(this->targetX, this->targetY, '.');
    }

    int tailIdx = this->offsetIndex(this->headIndex, -(startLength + this->score));

    int tailX = this->snakeCoordX[tailIdx];
    int tailY = this->snakeCoordY[tailIdx];

    if ((tailX != -1) && (tailY != -1)) {
        display->setElem(tailX, tailY, ' ');
        this->snakeCoordX[tailIdx] = -1;
        this->snakeCoordY[tailIdx] = -1;
    }

    int prevHead = this->prevIndex(this->headIndex);

    if ((this->snakeCoordX[prevHead] == this->snakeCoordX[this->headIndex])
        && (this->snakeCoordX[prevHead] == this->snakeCoordX[this->prevIndex(prevHead)])) {
        display->setElem(this->snakeCoordX[prevHead], this->snakeCoordY[prevHead], '-');
    } else if ((this->snakeCoordY[prevHead] == this->snakeCoordY[this->headIndex])
        && (this->snakeCoordY[prevHead] == this->snakeCoordY[this->prevIndex(prevHead)])) {
        display->setElem(this->snakeCoordX[prevHead], this->snakeCoordY[prevHead], '|');
    } else {
        display->setElem(this->snakeCoordX[prevHead], this->snakeCoordY[prevHead], '+');
    }

    if (this->snakeCoordX[prevHead] + 1 == this->snakeCoordX[this->headIndex]) {
        display->setElem(this->snakeCoordX[this->headIndex], this->snakeCoordY[this->headIndex], '>');
    } else if (this->snakeCoordX[prevHead] - 1 == this->snakeCoordX[this->headIndex]) {
        display->setElem(this->snakeCoordX[this->headIndex], this->snakeCoordY[this->headIndex], '<');
    } else if (this->snakeCoordY[prevHead] + 1 == this->snakeCoordY[this->headIndex]) {
        display->setElem(this->snakeCoordX[this->headIndex], this->snakeCoordY[this->headIndex], '^');
    } else /* if (this->snakeCoordY[prevHead] - 1 == this->snakeCoordY[this->headIndex]) */{
        display->setElem(this->snakeCoordX[this->headIndex], this->snakeCoordY[this->headIndex], 'v');
    }
}

void SnakeGame::setTarget() {
    this->targetX = -1, this->targetY = -1;

    while ((this->targetX != -1) && (this->targetY != -1)) {
        this->targetX = rand();
        this->targetY = rand();

        for (int snakeIndex = this->offsetIndex(this->headIndex, -(startLength + this->score - 1));
            snakeIndex != this->nextIndex(this->headIndex); snakeIndex = this->nextIndex(snakeIndex)) {
            if ((this->snakeCoordX[snakeIndex] == this->targetX) && (this->snakeCoordY[snakeIndex] == this->targetY)) {
                this->targetX = -1;
                this->targetY = -1;
            }
        }
    }
}

bool SnakeGame::getUserInput() {
    //ConsoleWindow *display = ConsoleWindow::getWindow ();

    Sleep(SnakeGame::startUpdateDuration_ms);
    return false;
}

bool SnakeGame::moveSnake() {
    ConsoleWindow *display = ConsoleWindow::getWindow();

    int newHeadX = this->snakeCoordX[this->headIndex] + this->dirX;
    int newHeadY = this->snakeCoordY[this->headIndex] + this->dirY;

    if ((newHeadX < 0) || (newHeadX >= display->getWidth()) || (newHeadY < 0) || (newHeadY >= display->getHeight())) {
        Sleep(5000);
        return true;
    }

    this->headIndex = this->nextIndex(this->headIndex);

    this->snakeCoordX[this->headIndex] = newHeadX;
    this->snakeCoordY[this->headIndex] = newHeadY;

    if ((newHeadX == targetX) && (newHeadY == targetY)) {
        score++;
        this->targetX = -1, this->targetY = -1;

        this->setTarget();
    }

    return false;
}

int SnakeGame::nextIndex(int index) {
    return this->offsetIndex(index, 1);
}

int SnakeGame::prevIndex(int index) {
    return this->offsetIndex(index, -1);
}

int SnakeGame::offsetIndex(int index, int offset) {
    return (index + offset) % SnakeGame::maxLength;
}
