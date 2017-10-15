/*
 * UserInput.h
 *
 *  Created on: Mar 11, 2016
 *      Author: Simon
 */

#ifndef USERINPUT_H_
#define USERINPUT_H_

namespace UserInput {
	enum ScreenColour {
		BLACK = 0
	};

	enum InputEnum {
		LEFT,
		RIGHT,
		UP,
		DOWN,
		ENTER,
		voidEntry
	};
}

typedef struct {
	bool keyPress;
	UserInput::InputEnum key;
} inputStruct;

#endif /* USERINPUT_H_ */
