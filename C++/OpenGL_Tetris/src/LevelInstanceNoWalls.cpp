/*
 * LevelInstanceInvisPieces.cpp
 *
 *  Created on: Nov 12, 2017
 *      Author: Simon
 */

#include "LevelInstanceNoWalls.hpp"

bool LevelInstanceNoWalls::testPieceMove(TetrisPiece &piece) const {
    std::pair<size_t, size_t> location = piece.getPieceLoc();

    for (size_t heightIndex = 0; heightIndex < piece.getHeight(); ++heightIndex) {
        for (size_t widthIndex = 0; widthIndex < piece.getWidth(); ++widthIndex) {

            if (piece(widthIndex, heightIndex) &&
                ((heightIndex + location.second >= this->window.getHeight()) ||
                this->window(widthIndex + location.first + this->window.getWidth(), heightIndex + location.second)))
            {
                return false;
            }
        }
    }

    return true;
}
