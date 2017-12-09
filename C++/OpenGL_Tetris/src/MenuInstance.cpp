/*
 * MenuInstance.cpp
 *
 *  Created on: Dec 8, 2017
 *      Author: Simon
 */

#include "MenuInstance.hpp"

bool MenuInstance::update(double deltaTime_ms, const UserInputStruct & userInput) {
	//Instance::getLogger().Log(LogLevel::Info, LOG_ARGS) << "Bla" << END;

	//int a = this->temp;

	INFO << "update" << END;
	return true;
}

bool MenuInstance::render(void) {
	//INFO << "render" << END;
	return true;
}

bool MenuInstance::renderText(void) {
	//INFO << "renderText" << END;
	return true;
}

bool MenuInstance::renderBackground(void) {
	//INFO << "renderBackground" << END;
	return true;
}

MenuInstance::~MenuInstance(void) {}


