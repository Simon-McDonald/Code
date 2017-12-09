/*
 * Instance.hpp
 *
 *  Created on: Dec 8, 2017
 *      Author: Simon
 */

#ifndef SRC_INSTANCE_HPP_
#define SRC_INSTANCE_HPP_

#include <UtilityManager.hpp>
#include <WorldManager.hpp>

class Instance : protected UtilityManager, protected WorldManager {
public:
	virtual bool update(double deltaTime_ms, const UserInputStruct & userInput) = 0;

	virtual bool render(void) = 0;
	virtual bool renderText(void) = 0;
	virtual bool renderBackground(void) = 0;

	virtual ~Instance(void) {};
};

#endif /* SRC_INSTANCE_HPP_ */
