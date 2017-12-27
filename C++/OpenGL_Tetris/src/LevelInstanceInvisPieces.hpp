/*
 * LevelInstanceInvisPieces.hpp
 *
 *  Created on: Nov 12, 2017
 *      Author: Simon
 */

#ifndef SRC_LEVELINSTANCEINVISPIECES_HPP_
#define SRC_LEVELINSTANCEINVISPIECES_HPP_

#include "LevelInstance.hpp"

/*
 * Cause the pieces that are set to be invisible.
 */
class LevelInstanceInvisPieces : public LevelInstance {
public:
    /*
     * Simply don't render the grid window.
     */
    virtual void renderMainWindow(GLfloat blockSize) override;
};

#endif /* SRC_LEVELINSTANCEINVISPIECES_HPP_ */
