/*
 * ConsoleWindow.h
 *
 *  Created on: Mar 7, 2016
 *      Author: Simon
 */

#ifndef CONSOLEWINDOW_H_
#define CONSOLEWINDOW_H_

#include <windows.h>

#include "UserInput.h"

class ConsoleWindow {
public:
    ~ConsoleWindow();
    static ConsoleWindow* getWindow();

    void setWindowSize(int width, int height);

    void setElem(int x, int y, char val);
    void setLineElem(int x, int y, char val, int length);
    void setString(int x, int y, const char *str, int len);
    void setTitleString(int y, const char *str, int len);
    void addBorder(int x1, int y1, int x2, int y2, int pattern);
    void clearScreen();

    void update();

    inputStruct getUserInput(bool block);
    inputStruct getUserInputDelay();

    int getWidth();
    int getHeight();

private:
    ConsoleWindow(int width, int height);

    void _addBorder(int x1, int y1, int x2, int y2, int pattern);
    void _setElem(int x, int y, char val);
    void _validateCoordinates(int x, int y);

    void _setLineElem(int x, int y, char val, int length, bool isHorizontal);
    void _setString(int x, int y, const char *str, int len, bool isHorizontal);

    RECT r;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    HANDLE hstdin;
    HANDLE hstdout;

    int width, height;
    char *screenBuffer;

    short screenStartY;
};

#endif /* CONSOLEWINDOW_H_ */
