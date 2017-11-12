/*
 * OpenGLController.h
 *
 *  Created on: Oct 14, 2015
 *      Author: Simon
 */

#ifndef OPENGLWINDOW_H_
#define OPENGLWINDOW_H_

#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "UtilityManager.hpp"

struct ButtonEventStruct {
	bool quit;
	bool space;
	bool left;
	bool right;
	bool up;
	bool down;

	bool rightMouse;
	bool leftMouse;
	bool middleMouse;
};

struct UserInputStruct {
	ButtonEventStruct onClick;
	ButtonEventStruct onDown;
	ButtonEventStruct onRelease;

	float mousePosX;
	float mousePosY;
	float prevMousePosX;
	float prevMousePosY;
};

class OpenGLWindow : UtilityManager {
public:
	OpenGLWindow(void);

	OpenGLWindow(const OpenGLWindow &orig) = delete;
	OpenGLWindow& operator=(const OpenGLWindow &orig) = delete;

	bool Initialise(void);

	int getHeight(void);
	int getWidth(void);

	SDL_Window* getWindow (void);
	SDL_GLContext* getContext (void);

	void userInput(void);
	UserInputStruct& getInput(void);
	void setMousePos(int x, int y);

	void clearWindow(void);
	void updateWindow(void);

	void cleanUp(void);

	~OpenGLWindow(void);

private:
	UserInputStruct userInputStruct;

	int width;
	int height;

	SDL_Window *window;
	SDL_GLContext context;

	Config::ConfigHeader header;

	bool WindowInitialise(void);
	void cleanUpWindow(void);
};

#endif /* OPENGLWINDOW_H_ */
