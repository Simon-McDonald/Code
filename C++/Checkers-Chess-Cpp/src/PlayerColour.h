/*
 * PlayerColours.h
 *
 *  Created on: Jan 8, 2016
 *      Author: Simon
 */

#ifndef SRC_PLAYERCOLOUR_H_
#define SRC_PLAYERCOLOUR_H_

#include <string>

using namespace std;

enum PlayerColour {
	white,
	black,
	none
};

inline PlayerColour flipPlayerColour (PlayerColour colour) {
	if (colour == PlayerColour::white) {
		return PlayerColour::black;
	} else if (colour == PlayerColour::black) {
		return PlayerColour::white;
	}
	return PlayerColour::none;
}

inline string playerColourString (PlayerColour colour) {
	string colourStr = "";

	switch (colour) {
	case PlayerColour::white:
		colourStr = "white";
		break;
	case PlayerColour::black:
		colourStr = "black";
		break;
	default:
		colourStr = "none";
		break;
	}
	return colourStr;
}

#endif /* SRC_PLAYERCOLOUR_H_ */
