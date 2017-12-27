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

#include "InstanceType.hpp"

/*
 * Abstract base class for object managed by the main game loop.
 */
class Instance : protected UtilityManager, protected WorldManager {
public:
    /*
     * Time and user input updates
     */
	virtual bool update(double deltaTime_ms, const UserInputStruct & userInput) = 0;

	/*
	 * Rendering with the 'TETRIS_PIECE_SHADER'
	 */
	virtual void render(void) = 0;

	/*
	 * Rendering with the 'TEXT_SHADER'
	 */
	virtual void renderText(void) = 0;

	/*
	 * Rendering with the 'BACKGROUND_SHADER'
	 */
	virtual void renderBackground(void) = 0;

	/*
	 * Instance to revert back to upon exit from current instance.
	 */
	virtual InstanceType endState(void) = 0;

	virtual ~Instance(void) {};
};

#endif /* SRC_INSTANCE_HPP_ */
