/*
 * OpenGLController.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: Simon
 */

#include "OpenGLWindow.hpp"

#include <iostream>

static ButtonEventStruct resetVersion;

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

	/*struct ButtonEventStruct {
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
	};	*/

	this->userInputStruct.onClick = resetVersion;
	this->userInputStruct.onRelease = resetVersion;

	SDL_Event windowEvent;
	while (SDL_PollEvent(&windowEvent)) {
		switch (windowEvent.type) {
			case SDL_QUIT:
				this->userInputStruct.onDown.quit = true;
				break;
			case SDL_KEYDOWN:
				switch(windowEvent.key.keysym.sym) {
					case SDLK_ESCAPE:
						this->userInputStruct.onDown.quit = true;
						this->userInputStruct.onClick.quit = true;
						break;
					case SDLK_SPACE:
						this->userInputStruct.onDown.space = true;
						this->userInputStruct.onClick.space = true;
						break;
					case SDLK_a:
					case SDLK_LEFT:
						this->userInputStruct.onDown.left = true;
						this->userInputStruct.onClick.left = true;
						break;
					case SDLK_d:
					case SDLK_RIGHT:
						this->userInputStruct.onDown.right = true;
						this->userInputStruct.onClick.right = true;
						break;
					case SDLK_w:
					case SDLK_UP:
						this->userInputStruct.onDown.up = true;
						this->userInputStruct.onClick.up = true;
						break;
					case SDLK_s:
					case SDLK_DOWN:
						this->userInputStruct.onDown.down = true;
						this->userInputStruct.onClick.down = true;
						break;
				}
				break;
			case SDL_KEYUP:
				switch(windowEvent.key.keysym.sym) {
					case SDLK_SPACE:
						this->userInputStruct.onDown.space = false;
						this->userInputStruct.onRelease.space = true;
						break;
					case SDLK_a:
					case SDLK_LEFT:
						this->userInputStruct.onDown.left = false;
						this->userInputStruct.onRelease.left = true;
						break;
					case SDLK_d:
					case SDLK_RIGHT:
						this->userInputStruct.onDown.right = false;
						this->userInputStruct.onRelease.right = true;
						break;
					case SDLK_w:
					case SDLK_UP:
						this->userInputStruct.onDown.up = false;
						this->userInputStruct.onRelease.up = true;
						break;
					case SDLK_s:
					case SDLK_DOWN:
						this->userInputStruct.onDown.down = false;
						this->userInputStruct.onRelease.down = true;
						break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				switch(windowEvent.button.button) {
					case SDL_BUTTON_LEFT:
						this->userInputStruct.onDown.leftMouse = true;
						this->userInputStruct.onClick.leftMouse = true;
						break;
					case SDL_BUTTON_MIDDLE:
						this->userInputStruct.onDown.middleMouse = true;
						this->userInputStruct.onClick.middleMouse = true;
						break;
					case SDL_BUTTON_RIGHT:
						this->userInputStruct.onDown.rightMouse = true;
						this->userInputStruct.onClick.rightMouse = true;
						break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch(windowEvent.button.button) {
					case SDL_BUTTON_LEFT:
						this->userInputStruct.onDown.leftMouse = false;
						this->userInputStruct.onRelease.leftMouse = true;
						break;
					case SDL_BUTTON_MIDDLE:
						this->userInputStruct.onDown.middleMouse = false;
						this->userInputStruct.onRelease.middleMouse = true;
						break;
					case SDL_BUTTON_RIGHT:
						this->userInputStruct.onDown.rightMouse = false;
						this->userInputStruct.onRelease.rightMouse = true;
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
