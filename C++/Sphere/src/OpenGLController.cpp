/*
 * OpenGLController.cpp
 *
 *  Created on: Oct 14, 2015
 *      Author: Simon
 */

#include "OpenGLController.h"
#include <iostream>
#include "ErrorCheck.h"

void OpenGLController::Initialise() {
	// Tell SDL which modules are needed
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	// Tell SDL that a forward compatible OpenGL 3.2 context is required
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Create the window
	this->window = SDL_CreateWindow("OpenGL", 100, 100, 800, 600, SDL_WINDOW_OPENGL);
	// Final argument:
	//  - SDL_WINDOW_OPENGL - Create a window ready for OpenGL
	//  - SDL_WINDOW_RESIZABLE - Create a resizable window
	//  - SDL_WINDOW_FULLSCREEN - Create a fullscreen window

	// Create the OpenGL context
	this->context = SDL_GL_CreateContext(window);
	CHECKERRORS();
	// Initialise GLEW
	glewExperimental = GL_TRUE;
	CHECKERRORS();
	glewInit();
	glEnable(GL_DEPTH_TEST);
	CHECKERRORS();

	this->userInputStruct.quit = false;
	this->userInputStruct.space = false;
	this->userInputStruct.left = false;
	this->userInputStruct.right = false;
	this->userInputStruct.up = false;
	this->userInputStruct.down = false;

	this->userInputStruct.rightMouse = false;
	this->userInputStruct.leftMouse = false;
	this->userInputStruct.middleMouse = false;

	this->userInputStruct.mousePosX = 0;
	this->userInputStruct.mousePosY = 0;
	this->userInputStruct.prevMousePosX = 0;
	this->userInputStruct.prevMousePosY = 0;
}

void OpenGLController::UserInput() {

	this->userInputStruct.prevMousePosX = this->userInputStruct.mousePosX;
	this->userInputStruct.prevMousePosY = this->userInputStruct.mousePosY;

	while (SDL_PollEvent(&this->windowEvent)) {
		switch (this->windowEvent.type) {
			case SDL_QUIT:
				this->userInputStruct.quit = true;
				break;
			case SDL_KEYDOWN:
				switch(this->windowEvent.key.keysym.sym) {
					case SDLK_ESCAPE:
						this->userInputStruct.quit = true;
						break;
					/*case SDLK_a:
						keyStates['a'] = true;
						break;
					case SDLK_s:
						keyStates['s'] = true;
						break;
					case SDLK_d:
						keyStates['d'] = true;
						break;
					case SDLK_w:
						keyStates['w'] = true;
						break;*/
					case SDLK_SPACE:
						this->userInputStruct.space = true;
						break;
					case SDLK_LEFT:
						this->userInputStruct.left = true;
						break;
					case SDLK_RIGHT:
						this->userInputStruct.right = true;
						break;
					case SDLK_UP:
						this->userInputStruct.up = true;
						break;
					case SDLK_DOWN:
						this->userInputStruct.down = true;
						break;
				}
				break;
			case SDL_KEYUP:
				switch(this->windowEvent.key.keysym.sym) {
					/*case SDLK_a:
						keyStates['a'] = true;
						break;
					case SDLK_s:
						keyStates['s'] = true;
						break;
					case SDLK_d:
						keyStates['d'] = true;
						break;
					case SDLK_w:
						keyStates['w'] = true;
						break;*/
					case SDLK_SPACE:
						this->userInputStruct.space = false;
						break;
					case SDLK_LEFT:
						this->userInputStruct.left = false;
						break;
					case SDLK_RIGHT:
						this->userInputStruct.right = false;
						break;
					case SDLK_UP:
						this->userInputStruct.up = false;
						break;
					case SDLK_DOWN:
						this->userInputStruct.down = false;
						break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				/*if (&this->windowEvent.button.button == SDL_BUTTON_LEFT) {
					this->userInputStruct.leftMouse = true;
				}
				if (&this->windowEvent.button.button == SDL_BUTTON_MIDDLE) {
					this->userInputStruct.middleMouse = true;
				}
				if (&this->windowEvent.button.button == SDL_BUTTON_RIGHT) {
					this->userInputStruct.rightMouse = true;
				}*/

				switch(this->windowEvent.button.button) {
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
				/*if (&this->windowEvent.button.button == SDL_BUTTON_LEFT) {
					this->userInputStruct.leftMouse = false;
				}
				if (&this->windowEvent.button.button == SDL_BUTTON_MIDDLE) {
					this->userInputStruct.middleMouse = false;
				}
				if (&this->windowEvent.button.button == SDL_BUTTON_RIGHT) {
					this->userInputStruct.rightMouse = false;
				}*/

				switch(this->windowEvent.button.button) {
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
				//std::cout << "The mouse moved!" << std::endl;

				/*this->userInputStruct.mousePosX =*/


				this->userInputStruct.mousePosX = this->windowEvent.motion.x;
				this->userInputStruct.mousePosY = this->windowEvent.motion.y;

				/*if( moving ){
					camera.lookat(float(Event->motion.x - winWidth/2),float(Event->motion.y - winHeight/2), MOUSE_SENSITIVITY, dt);
					SDL_EventState(SDL_MOUSEMOTION, SDL_IGNORE);
					SDL_WarpMouse(winWidth/2, winHeight/2);
					SDL_EventState(SDL_MOUSEMOTION, SDL_ENABLE);
				}*/
				break;
		}
	}
}

void OpenGLController::Update() {
	SDL_GL_SwapWindow(this->window);
}

void OpenGLController::Terminate() {
	SDL_GL_DeleteContext(this->context);
	SDL_Quit();
}
