/*
 * MenuInstance.hpp
 *
 *  Created on: Dec 8, 2017
 *      Author: Simon
 */

#ifndef SRC_MENUINSTANCE_HPP_
#define SRC_MENUINSTANCE_HPP_

#include "Instance.hpp"

class MenuInstance : public Instance {
public:
	virtual bool update(double deltaTime_ms, const UserInputStruct & userInput) override;

	virtual bool render(void) override;
	virtual bool renderText(void) override;
	virtual bool renderBackground(void) override;

	virtual ~MenuInstance(void);

protected:
	int temp;
};

#endif /* SRC_MENUINSTANCE_HPP_ */
