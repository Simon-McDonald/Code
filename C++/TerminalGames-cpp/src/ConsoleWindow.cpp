/*
 * ConsoleWindows.cpp
 *
 *  Created on: Mar 7, 2016
 *      Author: Simon
 */

#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "ConsoleWindow.h"

ConsoleWindow::ConsoleWindow(int width, int height) :
    screenStartY(-1) {
    HWND console = GetConsoleWindow();
    GetWindowRect(console, &this->r);

    MoveWindow(console, this->r.left, this->r.top, width, height, TRUE);

    RECT rNew;
    GetWindowRect(console, &rNew);

    this->hstdin = GetStdHandle( STD_INPUT_HANDLE);
    this->hstdout = GetStdHandle( STD_OUTPUT_HANDLE);

    GetConsoleScreenBufferInfo(this->hstdout, &this->csbi);

    this->width = (rNew.right - rNew.left) / 8 - 4;
    this->height = (rNew.bottom - rNew.top) / 12 - 5;
    this->screenBuffer = new char[(this->width + 1) * this->height];
    this->clearScreen();
} /* Window::ConsoleWindow::ConsoleWindow */

ConsoleWindow::~ConsoleWindow() {
    HWND console = GetConsoleWindow();

    MoveWindow(console, this->r.left, this->r.top, this->r.right - this->r.left, this->r.bottom - this->r.top,
    TRUE);

    FlushConsoleInputBuffer(this->hstdin);

    SetConsoleTextAttribute(this->hstdout, this->csbi.wAttributes);
} /* Window::ConsoleWindow::~ConsoleWindow */

ConsoleWindow* ConsoleWindow::getWindow() {
    static ConsoleWindow window(500, 400);
    return &window;
} /* Window::ConsoleWindow::getWindow */

void ConsoleWindow::update() {
    static bool firstDisplay = true;

    if (!firstDisplay) {
        COORD coord = {0, this->screenStartY};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    //sprintf (this->screenBuffer, "%i", this->screenStartY);
    printf("%.*s", (this->width + 1) * this->height - 1, screenBuffer);

    if (firstDisplay) {
        CONSOLE_SCREEN_BUFFER_INFO tempcsbi;
        GetConsoleScreenBufferInfo(this->hstdout, &tempcsbi);

        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &tempcsbi)) {
            this->screenStartY = tempcsbi.dwCursorPosition.Y - this->height + 1;
        }
        firstDisplay = false;
    }
} /* Window::ConsoleWindow::update */

void ConsoleWindow::setElem(int x, int y, char val) {
    this->_setElem(x + 1, y + 1, val);
} /* Window::ConsoleWindow::setElem */

void ConsoleWindow::_setElem(int x, int y, char val) {
    this->_validateCoordinates(x, y);
    this->screenBuffer[(this->height - y - 1) * (this->width + 1) + x] = val;
} /* Window::ConsoleWindow::_setElem */

void ConsoleWindow::setString(int x, int y, const char *str, int len) {
    x += 1, y += 1;
    this->_validateCoordinates(x, y);
    this->_validateCoordinates(x + len, y);

    memcpy(this->screenBuffer + (this->height - y - 1) * (this->width + 1) + x, str, len);
} /* Window::ConsoleWindow::setString */

void ConsoleWindow::addBorder(int x1, int y1, int x2, int y2, int pattern) {
    this->_addBorder(x1 + 1, y1 + 1, x2 + 1, y2 + 1, pattern);
} /* Window::ConsoleWindow::addBorder */

void ConsoleWindow::_addBorder(int x1, int y1, int x2, int y2, int pattern) {

    if (x1 < x2) {
        x1 -= x2;
        x2 += x1;
        x1 = -x1 + x2;
    }
    if (y1 < y2) {
        y1 -= y2;
        y2 += y1;
        y1 = -y1 + y2;
    }

    static char cornerTL[] = {' ', '+', '*', '/', '\\'};
    static char cornerTR[] = {' ', '+', '*', '\\', '/'};
    static char cornerBL[] = {' ', '+', '*', '\\', '/'};
    static char cornerBR[] = {' ', '+', '*', '/', '\\'};
    static char sideT[] = {' ', '-', '*', '-', '^'};
    static char sideR[] = {' ', '|', '*', '|', '>'};
    static char sideB[] = {' ', '-', '*', '-', 'v'};
    static char sideL[] = {' ', '|', '*', '|', '<'};

    int numPatterns = sizeof(cornerTL) / sizeof(cornerTL[0]);

    this->_validateCoordinates(x1, y1);
    this->_validateCoordinates(x2, y2);

    assert(
        (sizeof(cornerTR) == sizeof(cornerTL)) && (sizeof(cornerBL) == sizeof(cornerTL))
            && (sizeof(cornerBR) == sizeof(cornerTL)) && (sizeof(sideT) == sizeof(cornerTL))
            && (sizeof(sideR) == sizeof(cornerTL)) && (sizeof(sideB) == sizeof(cornerTL))
            && (sizeof(sideL) == sizeof(cornerTL)));
    assert(pattern >= 0 && pattern < numPatterns);

    for (int col = x1; col != x2; col += (x2 - x1) / abs(x2 - x1)) {
        this->_setElem(col, y1, sideT[pattern]);
        this->_setElem(col, y2, sideB[pattern]);
    }

    for (int row = y1; row != y2; row += (y2 - y1) / abs(y2 - y1)) {
        this->_setElem(x1, row, sideL[pattern]);
        this->_setElem(x2, row, sideR[pattern]);
    }

    this->_setElem(x1, y2, cornerBL[pattern]);
    this->_setElem(x2, y2, cornerBR[pattern]);
    this->_setElem(x2, y1, cornerTR[pattern]);
    this->_setElem(x1, y1, cornerTL[pattern]);
} /* Window::ConsoleWindow::_addBorder */

void ConsoleWindow::clearScreen() {
    memset(this->screenBuffer, ' ', ((this->width + 1) * this->height));

    for (int lineIndex = 0; lineIndex < this->height; lineIndex++) {
        this->screenBuffer[lineIndex * (this->width + 1) + this->width] = '\n';
    }
    this->_addBorder(0, this->height - 1, this->width - 1, 0, 1);
} /* Window::ConsoleWindow::clearScreen */

void ConsoleWindow::setLineElem(int x, int y, char val, int length) {
    this->_setLineElem(x + 1, y + 1, val, length, true);
} /* Window::ConsoleWindow::setLineElem */

void ConsoleWindow::setTitleString(int y, const char *str, int len) {

    int x = this->getWidth() / 2 - len / 2;

    this->setLineElem(x - 1, y + 1, '*', len + 2);
    this->setElem(x - 1, y, '*');
    this->setString(x, y, str, len);
    this->setElem(x + len, y, '*');
    this->setLineElem(x - 1, y - 1, '*', len + 2);
} /* Window::ConsoleWindow::setTitleString */

void ConsoleWindow::_setLineElem(int x, int y, char val, int length, bool isHorizontal) {
    this->_validateCoordinates(x, y);
    this->_validateCoordinates(x + isHorizontal ? 0 : (length - 1), y + isHorizontal ? (length - 1) : 0);

    if (isHorizontal) {
        memset(this->screenBuffer + (this->height - y - 1) * (this->width + 1) + x, val, length);
    } else {
        // TODO
        memset(this->screenBuffer + (this->height - y - 1) * (this->width + 1) + x, val, length);
    }
} /* Window::ConsoleWindow::_setLineElem */

void ConsoleWindow::_setString(int x, int y, const char *str, int length, bool isHorizontal) {
    this->_validateCoordinates(x, y);
    this->_validateCoordinates(x + isHorizontal ? 0 : (length - 1), y + isHorizontal ? (length - 1) : 0);

    if (isHorizontal) {
        memcpy(this->screenBuffer + (this->height - y - 1) * (this->width + 1) + x, str, length);
    } else {
        // TODO
        memcpy(this->screenBuffer + (this->height - y - 1) * (this->width + 1) + x, str, length);
    }
} /* Window::ConsoleWindow::_setString */

inputStruct ConsoleWindow::getUserInput(bool block) {
    inputStruct returnVal = {false, UserInput::voidEntry};

    while (WaitForSingleObject(this->hstdin, 1000) == WAIT_TIMEOUT) {
    }

    DWORD numEvents = 0;
    GetNumberOfConsoleInputEvents(this->hstdin, &numEvents);

    DWORD numInputs = 0;
    INPUT_RECORD inputs[10];

    bool status = ReadConsoleInput(this->hstdin, &inputs[0], 1, &numInputs);

    if (status) {
        switch (inputs[0].EventType) {
        case FOCUS_EVENT:
            std::cout << "FOCUS_EVENT" << std::endl;
            break;
        case KEY_EVENT: {
            KEY_EVENT_RECORD potato = inputs[0].Event.KeyEvent;

            returnVal.keyPress = potato.bKeyDown;

            switch (potato.wVirtualKeyCode) {
            case VK_UP:
                returnVal.key = UserInput::UP;
                return returnVal;
            case VK_DOWN:
                returnVal.key = UserInput::DOWN;
                return returnVal;
            case VK_RETURN:
                returnVal.key = UserInput::ENTER;
                return returnVal;
            }
        }
            break;
        case MENU_EVENT:
            //std::cout << "MENU_EVENT" << std::endl;
            break;
        case MOUSE_EVENT:
            //std::cout << "MOUSE_EVENT" << std::endl;
            break;
        case WINDOW_BUFFER_SIZE_EVENT:
            //std::cout << "WINDOW_BUFFER_SIZE_EVENT" << std::endl;
            break;
        default:
            //std::cout << "default" << std::endl;
            break;
        }
    } else {
        std::cout << "Failed to get some input!!" << std::endl;
    }

    FlushConsoleInputBuffer(this->hstdin);

    return returnVal;
} /* Window::ConsoleWindow::getUserInput */

void ConsoleWindow::_validateCoordinates(int x, int y) {
    if (x < 0 || x >= this->width) {
        std::cout << "x-index error, bounds [0, " << this->width << "), actual - " << x << std::endl;
    }
    if (y < 0 || y >= this->height) {
        std::cout << "y-index error, bounds [0, " << this->height << "), actual - " << y << std::endl;
    }

    assert(x >= 0 && x < this->width);
    assert(y >= 0 && y < this->height);
} /* Window::ConsoleWindow::_validateCoordinates */

int ConsoleWindow::getWidth() {
    return this->width - 2;
} /* Window::ConsoleWindow::getWidth */

int ConsoleWindow::getHeight() {
    return this->height - 2;
} /* Window::ConsoleWindow::getHeight */
