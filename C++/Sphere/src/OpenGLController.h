/*
 * OpenGLController.h
 *
 *  Created on: Oct 14, 2015
 *      Author: Simon
 */

#ifndef OPENGLCONTROLLER_H_
#define OPENGLCONTROLLER_H_

#define GLEW_STATIC
#include <GL/glew.h>

#include <SDL.h>
#include <SDL_opengl.h>

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

	int mousePosX;
	int mousePosY;
	int prevMousePosX;
	int prevMousePosY;
};

class OpenGLController {

public:
	void Initialise();
	void UserInput();
	void Update();
	void Terminate();

	UserInputStruct userInputStruct;

private:
	SDL_Event windowEvent;
	SDL_GLContext context;
	SDL_Window* window;
};

#endif /* OPENGLCONTROLLER_H_ */
