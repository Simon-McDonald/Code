/*
 * LevelInstanceInvisPieces.hpp
 *
 *  Created on: Nov 12, 2017
 *      Author: Simon
 */

#ifndef SRC_LEVELINSTANCENOWALLS_HPP_
#define SRC_LEVELINSTANCENOWALLS_HPP_

#include "LevelInstance.hpp"

/*
 * Remove the vertical barriers and allow wrap around.
 */
class LevelInstanceNoWalls : public LevelInstance {
protected:
    /*
     * Remove checks on the vertical sizes of the grid window.
     */
    virtual bool testPieceMove(TetrisPiece &piece) const override;
};

#endif /* SRC_LEVELINSTANCENOWALLS_HPP_ */
