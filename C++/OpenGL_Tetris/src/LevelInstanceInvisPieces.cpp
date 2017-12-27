/*
 * LevelInstanceInvisPieces.cpp
 *
 *  Created on: Nov 12, 2017
 *      Author: Simon
 */

#include "LevelInstanceInvisPieces.hpp"

void LevelInstanceInvisPieces::renderMainWindow(GLfloat blockSize) {
    this->window.setGridDimsInShader();
    this->setMainWindowUniforms(blockSize);

    this->getShaderManager().bindVector2i("uPieceOffset", this->currentPiece.getPieceLoc().first, this->currentPiece.getPieceLoc().second);
    this->currentPiece.RenderPiece();
}
