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
#include "GridWindow.hpp"

class MenuInstance : public Instance {
public:
    MenuInstance(std::vector<std::pair<InstanceType, std::string>> itemNames);

	virtual bool update(double deltaTime_ms, const UserInputStruct & userInput) override;

	virtual void render(void) override;
	virtual void renderText(void) override;
	virtual void renderBackground(void) override;

	virtual InstanceType endState(void);

	virtual ~MenuInstance(void);

protected:
	static const GLColour<GLfloat> selectedColour;
	static const GLColour<GLfloat> defaultColour;

    TextImage textImageInst;
	std::vector<RenderableText> items;
	std::vector<InstanceType> instanceTypes;
	size_t menuIndex;

	GridWindow title;

	GLuint blockTextureId;

	void updateSelectedColours(size_t oldSelectedIdx, size_t newSelectedIdx);
};

#endif /* SRC_MENUINSTANCE_HPP_ */
