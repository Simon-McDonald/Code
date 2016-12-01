/*
 * OpenGLController.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: Simon
 */

#include <iostream>

#include "OpenGLWindow.h"

OpenGLWindow::OpenGLWindow(void) : userInputStruct(),
	width(0), height(0), window(nullptr), context(nullptr), header("WINDOW") {
} /* OpenGLWindow::OpenGLController */

bool OpenGLWindow::Initialise(void) {
	if (SDL_Init (SDL_INIT_EVERYTHING) == -1) {
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	std::string widthKey = "window_width";
	if (this->getConfig().containsHeader(this->header) &&
		this->getConfig().containsKey(this->header, widthKey)) {
		this->width = this->getConfig().getInt(this->header, widthKey);
	} else {
		this->width = 800;
	}

	std::string heightKey = "window_height";
	if (this->getConfig().containsHeader(this->header) &&
		this->getConfig().containsKey(this->header, heightKey)) {
		this->height = this->getConfig().getInt(this->header, heightKey);
	} else {
		this->height = 600;
	}

	std::cout << "Width: " << this->width << ", height: " << this->height << std::endl;

	this->window = SDL_CreateWindow("OpenGL", 100, 100,
			this->width, this->height, SDL_WINDOW_OPENGL);
	if (!window) {
		return false;
	}
	this->context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	return true;
} /* OpenGLWindow::Initialise */

int OpenGLWindow::getWidth(void) {
	return this->width;
} /* OpenGLWindow::getWidth */

int OpenGLWindow::getHeight(void) {
	return this->height;
} /* OpenGLWindow::getWidth */

SDL_Window* OpenGLWindow::getWindow(void) {
	return this->window;
} /* WindowManager::getWindow */

SDL_GLContext* OpenGLWindow::getContext(void) {
	return &this->context;
} /* WindowManager::getContext */

void OpenGLWindow::userInput (void) {
	this->userInputStruct.prevMousePosX = this->userInputStruct.mousePosX;
	this->userInputStruct.prevMousePosY = this->userInputStruct.mousePosY;

	SDL_Event windowEvent;
	while (SDL_PollEvent(&windowEvent)) {
		switch (windowEvent.type) {
			case SDL_QUIT:
				this->userInputStruct.quit = true;
				break;
			case SDL_KEYDOWN:
				switch(windowEvent.key.keysym.sym) {
					case SDLK_ESCAPE:
						this->userInputStruct.quit = true;
						break;
					case SDLK_SPACE:
						this->userInputStruct.space = true;
						break;
					case SDLK_a:
					case SDLK_LEFT:
						this->userInputStruct.left = true;
						break;
					case SDLK_d:
					case SDLK_RIGHT:
						this->userInputStruct.right = true;
						break;
					case SDLK_w:
					case SDLK_UP:
						this->userInputStruct.up = true;
						break;
					case SDLK_s:
					case SDLK_DOWN:
						this->userInputStruct.down = true;
						break;
				}
				break;
			case SDL_KEYUP:
				switch(windowEvent.key.keysym.sym) {
					case SDLK_SPACE:
						this->userInputStruct.space = false;
						break;
					case SDLK_a:
					case SDLK_LEFT:
						this->userInputStruct.left = false;
						break;
					case SDLK_d:
					case SDLK_RIGHT:
						this->userInputStruct.right = false;
						break;
					case SDLK_w:
					case SDLK_UP:
						this->userInputStruct.up = false;
						break;
					case SDLK_s:
					case SDLK_DOWN:
						this->userInputStruct.down = false;
						break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch(windowEvent.button.button) {
					case SDL_BUTTON_LEFT:
						this->userInputStruct.leftMouse = true;
						break;
					case SDL_BUTTON_MIDDLE:
						this->userInputStruct.middleMouse = true;
						break;
					case SDL_BUTTON_RIGHT:
						this->userInputStruct.rightMouse = true;
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch(windowEvent.button.button) {
					case SDL_BUTTON_LEFT:
						this->userInputStruct.leftMouse = false;
						break;
					case SDL_BUTTON_MIDDLE:
						this->userInputStruct.middleMouse = false;
						break;
					case SDL_BUTTON_RIGHT:
						this->userInputStruct.rightMouse = false;
						break;
				}
				break;
			case SDL_MOUSEMOTION:
				this->userInputStruct.mousePosX = windowEvent.motion.x;
				this->userInputStruct.mousePosY = windowEvent.motion.y;
				break;
		}
	}
} /* OpenGLWindow::UserInput */

UserInputStruct& OpenGLWindow::getInput(void) {
	return this->userInputStruct;
} /* OpenGLWindow::getUserInput */

void OpenGLWindow::setMousePos(int x, int y) {
	SDL_WarpMouseInWindow(this->getWindow(), x, y);
	this->userInputStruct.mousePosX = x;
	this->userInputStruct.mousePosY = y;
} /* OpenGLWindow::setMousePos */

void OpenGLWindow::clearWindow(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
} /* OpenGLWindow::clearWindow */

void OpenGLWindow::updateWindow(void) {
	SDL_GL_SwapWindow (this->getWindow());
} /* OpenGLWindow::updateWindow */

void OpenGLWindow::cleanUp(void) {
	SDL_GL_DeleteContext(this->context);
	SDL_Quit();
} /* OpenGLWindow::cleanUp */

OpenGLWindow::~OpenGLWindow(void) {
} /* OpenGLWindow::~OpenGLWindow */
