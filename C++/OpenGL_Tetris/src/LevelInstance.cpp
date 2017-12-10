/*
 * LevelInstance.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: Simon
 */

#include "LevelInstance.hpp"

const double LevelInstance::minTimer_ms = 150;
const float LevelInstance::stringStartX = 85.0 / 225.0;
const double LevelInstance::pieceMoveDelta_ms = 50.0;
const float LevelInstance::aspectRatio = 1.0;

LevelInstance::LevelInstance(void) :
		window{18, 10},
		currentPiece(TetrisPiece::generateNextTetrisPiece()),
		nextPiece(TetrisPiece::generateNextTetrisPiece()),
		timerDuration_ms(1000),
		currentTimer_ms(timerDuration_ms),
		timerChangeScaling(50),
		textImageInst("TEXT"),
	    labelsText(&this->textImageInst, "Pieces:\nRows:\nScore:", stringStartX,
	        -0.2, 1.5, 1.5, TextImage::TextAlignment::LEFT,
	        TextImage::TextAlignment::TOP, 0.0, 0.05, GLColour<GLfloat>{1.0, 0.0, 0.0}),
	    valuesText(&this->textImageInst, " ", stringStartX + 0.5,
	        -0.25, 1.5, 1.5, TextImage::TextAlignment::RIGHT,
	        TextImage::TextAlignment::TOP, 0.0, 0.05, GLColour<GLfloat>{1.0, 0.0, 0.0}),
		piecesSet(0),
		rowsCleared(0),
		playerPoints(0),
		downButtonDuration(0.0),
		leftButtonDuration(0.0),
		rightButtonDuration(0.0) {

	currentPiece.setPieceLoc(4, 15);
	this->resetOutputStrings();
}

bool LevelInstance::update(double deltaTime_ms, const UserInputStruct &userInput) {
	this->currentTimer_ms -= deltaTime_ms;

	// Implement user input, if false indicates that input locks piece (i.e. push down but piece can't move down).
	if (!this->implementUserInput(userInput, deltaTime_ms)) {
		this->resetMoveTimer();
		this->lockCurrentPiece();

		if (!this->generateNextPiece()) {
			// game over
			return false;
		}

	} else if (this->currentTimer_ms <= 0) {
		this->resetMoveTimer();

		if (!this->moveCurrentPieceDown()) {
			this->lockCurrentPiece();

			if (!this->generateNextPiece()) {
				// game over
				return false;
			}
		}
	}

	return true;
}

bool LevelInstance::moveCurrentPieceDown() {
	TetrisPiece tempPiece = this->currentPiece;
	tempPiece.movePieceDown();

	return this->testImplementPieceMove(tempPiece);
}

bool LevelInstance::testImplementPieceMove(TetrisPiece &piece) {
	if (testPieceMove(piece)) {
		this->currentPiece = piece;

		return true;
	}
	return false;
}

void LevelInstance::resetMoveTimer(void) {
	this->currentTimer_ms = this->timerDuration_ms;
}

bool LevelInstance::lockCurrentPiece() {
	size_t numRows = this->window.lockNextPiece(this->currentPiece, this->currentPiece.getPieceLoc());

	this->playerPoints += this->pointsForRow(numRows);
	this->rowsCleared += numRows;
	this->piecesSet += 1;
	this->resetOutputStrings();

	this->timerDuration_ms -= this->timerChangeScaling * numRows;
	if (this->timerDuration_ms < minTimer_ms) {
		this->timerDuration_ms = minTimer_ms;
	}

	return numRows > 0;
}

bool LevelInstance::generateNextPiece() {
	this->currentPiece = this->nextPiece;
	this->currentPiece.setPieceLoc(4, 15);

	this->nextPiece = TetrisPiece::generateNextTetrisPiece();

	return this->testPieceMove(this->currentPiece);
}

bool LevelInstance::render(void) {


	GLfloat windowPixelHeight = (float) this->getWindow().getHeight();
	GLfloat windowPixelWidth = (float) this->getWindow().getWidth();

	GLfloat windowCentre[] = {180.0 / 540.0, 0.5};
	GLfloat windowSize[] = {300.0 / 540.0, 540.0 / 600.0};

	GLfloat blockSizes[] = {windowSize[0] * windowPixelWidth / (this->window.getWidth()),
							windowSize[1] * windowPixelHeight / (this->window.getHeight() * aspectRatio)};

	GLfloat minBlockSize = (blockSizes[0] > blockSizes[1]) ? blockSizes[1] : blockSizes[0];

	GLfloat newWindowSize[] = {minBlockSize * this->window.getWidth() / windowPixelWidth,
			 	 	 	 	 aspectRatio * minBlockSize * this->window.getHeight() / windowPixelHeight};

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
	this->window.render();

	// Render the next piece
	this->getShaderManager().bindVector2i("uWindowDims", 5, 5);

	this->getShaderManager().bindVector2("uWindowSize", tempWindowSize[0] * 2, tempWindowSize[1] * 2);
	this->getShaderManager().bindVector2("uWindowPos", 2 * tempWindowCentre[0] - 1 - tempWindowSize[0],
														2 * tempWindowCentre[1] - 1 - tempWindowSize[1]);
	this->nextPiece.RenderPiece();

	return true;
}

bool LevelInstance::renderBackground(void) {
	GLfloat windowPixelHeight = (float) this->getWindow().getHeight();
	GLfloat windowPixelWidth = (float) this->getWindow().getWidth();

	GLfloat windowCentre[] = {180.0 / 540.0, 0.5};
	GLfloat windowSize[] = {300.0 / 540.0, 540.0 / 600.0};

	GLfloat blockSizes[] = {windowSize[0] * windowPixelWidth / (this->window.getWidth()),
							windowSize[1] * windowPixelHeight / (this->window.getHeight() * aspectRatio)};

	GLfloat minBlockSize = (blockSizes[0] > blockSizes[1]) ? blockSizes[1] : blockSizes[0];

	GLfloat newWindowSize[] = {minBlockSize * this->window.getWidth() / windowPixelWidth,
			 	 	 	 	 aspectRatio * minBlockSize * this->window.getHeight() / windowPixelHeight};

	// Calculate the dimensions for the next piece
	GLfloat tempWindowBlocks[] = {5.0, 5.0};
	GLfloat tempWindowSize[] = {minBlockSize * tempWindowBlocks[0] / windowPixelWidth,
			 	 	 	 	 aspectRatio * minBlockSize * tempWindowBlocks[1] / windowPixelHeight};
	GLfloat tempWindowCentre[] = {450 / 540.0, 480.0 / 600.0};

	std::vector<GLfloat> borderPoints = {windowCentre[0], windowCentre[1], newWindowSize[0], newWindowSize[1],
										 tempWindowCentre[0], tempWindowCentre[1], tempWindowSize[0], tempWindowSize[1],
										 0.82, 0.32, 0.32, 0.3};

	DataBuffer<GLfloat, 2, 2> borderBuffer(3, &borderPoints[0]);

	borderBuffer.manageRender(1, 2);
	return true;
}

bool LevelInstance::renderText(void) {
    labelsText.Render();
    valuesText.Render();

	return true;
}

bool LevelInstance::testPieceMove(TetrisPiece &piece) {
	std::pair<size_t, size_t> location = piece.getPieceLoc();

	for (size_t heightIndex = 0; heightIndex < piece.getHeight(); ++heightIndex) {
		for (size_t widthIndex = 0; widthIndex < piece.getWidth(); ++widthIndex) {

			if (piece(widthIndex, heightIndex) &&
				(((widthIndex + location.first >= this->window.getWidth()) || (heightIndex + location.second >= this->window.getHeight())) ||
				this->window(widthIndex + location.first, heightIndex + location.second)))
			{
				return false;
			}
		}
	}

	return true;
}

bool LevelInstance::implementUserInput(const UserInputStruct &input, double deltaTime_ms) {
	bool result = true;

	if (input.onClick.up) {
		TetrisPiece tempPiece = this->currentPiece;
		tempPiece.rotateCCW();
		this->testImplementRotation(tempPiece);
	}

	if (input.onClick.down || (downButtonDuration > 0.75 * LevelInstance::pieceMoveDelta_ms)) {
		this->downButtonDuration = 0.0;
		TetrisPiece tempPiece = this->currentPiece;
		tempPiece.movePieceDown();
		result = this->testImplementPieceMove(tempPiece);
		this->resetMoveTimer();
	} else if (input.onDown.down) {
		downButtonDuration += deltaTime_ms;
	} else if (input.onRelease.down) {
		downButtonDuration = 0.0;
	}

	if (input.onDown.left && input.onDown.right) {
		this->leftButtonDuration = 0.0;
		this->rightButtonDuration = 0.0;
	} else {
		if (input.onClick.left || (leftButtonDuration > 1.5 * LevelInstance::pieceMoveDelta_ms)) {
			leftButtonDuration = 0.0;
			TetrisPiece tempPiece = this->currentPiece;
			tempPiece.movePieceLeft();
			this->testImplementPieceMove(tempPiece);
		} else if (input.onDown.left) {
			leftButtonDuration += deltaTime_ms;
		}

		if (input.onClick.right || (rightButtonDuration > 1.5 * LevelInstance::pieceMoveDelta_ms)) {
			this->rightButtonDuration = 0.0;
			TetrisPiece tempPiece = this->currentPiece;
			tempPiece.movePieceRight();
			this->testImplementPieceMove(tempPiece);
		} else if (input.onDown.right) {
			rightButtonDuration += deltaTime_ms;
		}
	}
	if (input.onRelease.left) {
		leftButtonDuration = 0.0;
	}
	if (input.onRelease.right) {
		rightButtonDuration = 0.0;
	}

	return result;
}

bool LevelInstance::testImplementRotation(TetrisPiece &piece) {
	if (this->testImplementPieceMove(piece)) {
		return true;
	}

	piece.movePieceLeft();
	if (this->testImplementPieceMove(piece)) {
		return true;
	}

	piece.movePieceRight().movePieceRight();
	if (this->testImplementPieceMove(piece)) {
		return true;
	}

	return false;
}

unsigned LevelInstance::pointsForRow(size_t numRowsCleared) {
	return numRowsCleared * numRowsCleared;
}

void LevelInstance::resetOutputStrings(void) {
    valuesText.updateText(std::to_string(this->piecesSet) + "\n" +
        std::to_string(this->rowsCleared) + "\n" + std::to_string(this->playerPoints));
}

LevelInstance::~LevelInstance(void) {
	INFO << "Killing the level!" << END;
}
