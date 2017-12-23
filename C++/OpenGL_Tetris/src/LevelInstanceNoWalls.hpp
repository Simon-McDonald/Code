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
 *
 */
class LevelInstanceNoWalls : public LevelInstance {
protected:
    virtual bool testPieceMove(TetrisPiece &piece) const override;
};

#endif /* SRC_LEVELINSTANCENOWALLS_HPP_ */
