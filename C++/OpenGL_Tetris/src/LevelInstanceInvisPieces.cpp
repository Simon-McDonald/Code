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

    this->getShaderManager().bindVector2i("uPieceOffset", 0, 0);
    this->window.render();
}

/*void LevelInstanceInvisPieces::render(void) {
    this->getShaderManager().setUniformInt("uGridWidth", this->window.getWidth());

	GLfloat aspectRatio = 1.0;

	GLfloat windowPixelHeight = (float) this->getWindow().getHeight();
	GLfloat windowPixelWidth = (float) this->getWindow().getWidth();

	GLfloat windowCentre[] = {180.0 / 540.0, 0.5};
	GLfloat windowSize[] = {300.0 / 540.0, 540.0 / 600.0};

	GLfloat blockSizes[] = {windowSize[0] * windowPixelWidth / (this->window.getWidth()),
							windowSize[1] * windowPixelHeight / (this->window.getHeight() * aspectRatio)};

//	INFO << "blockSizes: " << (windowSize * windowPixel)>

	GLfloat minBlockSize = (blockSizes[0] > blockSizes[1]) ? blockSizes[1] : blockSizes[0];

	//INFO << "minBlockSize: " << minBlockSize << END;

	GLfloat newWindowSize[] = {minBlockSize * this->window.getWidth() / windowPixelWidth,
			 	 	 	 	 aspectRatio * minBlockSize * this->window.getHeight() / windowPixelHeight};

	//INFO << "newWindowSize: " << (newWindowSize[0] * windowPixelWidth) << ", " << (newWindowSize[1] * windowPixelHeight) << END;

	// Calculate the dimensions for the next piece
	GLfloat tempWindowBlocks[] = {5.0, 5.0};
	GLfloat tempWindowSize[] = {minBlockSize * tempWindowBlocks[0] / windowPixelWidth,
			 	 	 	 	 aspectRatio * minBlockSize * tempWindowBlocks[1] / windowPixelHeight};
	GLfloat tempWindowCentre[] = {450 / 540.0, 480.0 / 600.0};






	// Render the game grid blocks and the current piece
	this->currentPiece.prepShaderTextures();

	this->window.setGridDimsInShader();

	this->getShaderManager().bindVector2("uWindowSize", newWindowSize[0] * 2, newWindowSize[1] * 2);
	this->getShaderManager().bindVector2("uWindowPos", 2 * windowCentre[0] - 1 - newWindowSize[0],
														2 * windowCentre[1] - 1 - newWindowSize[1]);
	this->getShaderManager().bindVector2i("uPieceOffset", this->currentPiece.getPieceLoc().first, this->currentPiece.getPieceLoc().second);
	this->currentPiece.RenderPiece();

	this->getShaderManager().bindVector2i("uPieceOffset", 0, 0);
	//this->window.render();

	// Render the next piece
	this->getShaderManager().bindVector2i("uWindowDims", 5, 5);

	this->getShaderManager().bindVector2("uWindowSize", tempWindowSize[0] * 2, tempWindowSize[1] * 2);
	this->getShaderManager().bindVector2("uWindowPos", 2 * tempWindowCentre[0] - 1 - tempWindowSize[0],
														2 * tempWindowCentre[1] - 1 - tempWindowSize[1]);
	this->nextPiece.RenderPiece();
}
*/

