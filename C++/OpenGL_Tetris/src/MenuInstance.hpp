/*
 * MenuInstance.hpp
 *
 *  Created on: Dec 8, 2017
 *      Author: Simon
 */

#ifndef SRC_MENUINSTANCE_HPP_
#define SRC_MENUINSTANCE_HPP_

#include "Instance.hpp"
#include "TextImage.hpp"

class MenuInstance : public Instance {
public:
    MenuInstance(std::vector<std::string> itemNames);

	virtual bool update(double deltaTime_ms, const UserInputStruct & userInput) override;

	virtual bool render(void) override;
	virtual bool renderText(void) override;
	virtual bool renderBackground(void) override;

	virtual ~MenuInstance(void);

protected:
	static const GLColour<GLfloat> selectedColour;
	static const GLColour<GLfloat> defaultColour;

    TextImage textImageInst;
	std::vector<RenderableText> items;
	size_t menuIndex;

	void updateSelectedColours(size_t oldSelectedIdx, size_t newSelectedIdx);
};

#endif /* SRC_MENUINSTANCE_HPP_ */
