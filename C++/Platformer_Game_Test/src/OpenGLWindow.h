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

#include "UtilityManager.h"

struct UserInputStruct {
	bool quit;
	bool space;
	bool left;
	bool right;
	bool up;
	bool down;

	bool rightMouse;
	bool leftMouse;
	bool middleMouse;

	float mousePosX;
	float mousePosY;
	float prevMousePosX;
	float prevMousePosY;
};

class OpenGLWindow : UtilityManager {
public:
	OpenGLWindow(void);

	bool Initialise(void);

	int getHeight(void);
	int getWidth(void);

	SDL_Window* getWindow (void);
	SDL_GLContext* getContext (void);

	void userInput(void);
	UserInputStruct& getInput(void);
	void setMousePos(int x, int y);

	void setClearColour(GLfloat, GLfloat, GLfloat, GLfloat);
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
};

#endif /* OPENGLWINDOW_H_ */
