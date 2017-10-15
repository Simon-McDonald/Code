/*
 * LevelInstance.cpp
 *
 *  Created on: Jul 9, 2017
 *      Author: Simon
 */

#include "LevelInstance.hpp"

const double LevelInstance::minTimer_ms = 100;

LevelInstance::LevelInstance(void) :
		window({18, 10}),
		currentPiece(TetrisPiece::generateNextTetrisPiece()),
		nextPiece(TetrisPiece::generateNextTetrisPiece()),
		timerDuration_ms(1000),
		currentTimer_ms(timerDuration_ms),
		timerChangeScaling(50),
		textImageInst("TEXT"),
		piecesSetText(&this->textImageInst, " ", (60.0 / 225.0), -0.3, 1.5),
		rowsClearedText(&this->textImageInst, " ", (60.0 / 225.0), -0.5, 1.5),
		playerPointsText(&this->textImageInst, " ", (60.0 / 225.0), -0.7, 1.5),
		piecesSet(0),
		rowsCleared(0),
		playerPoints(0) {

	currentPiece.setPieceLoc(0, 6);
	this->resetOutputStrings();
}

bool LevelInstance::update(double deltaTime_ms, const UserInputStruct &userInput) {
	this->currentTimer_ms -= deltaTime_ms;

	// Implement user input, if false indicates that input locks piece (i.e. push down but piece can't move down).
	if (!this->implementUserInput(userInput)) {
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
	this->currentPiece.setPieceLoc(0, 8);

	this->nextPiece = TetrisPiece::generateNextTetrisPiece();

	return this->testPieceMove(this->currentPiece);
}

bool LevelInstance::render(void) {
	GLfloat aspectRatio = 0.75;

	GLfloat windowPixelHeight = (float) this->getWindow().getHeight();
	GLfloat windowPixelWidth = (float) this->getWindow().getWidth();

	GLfloat windowCentre[] = {142.5 / 450.0, 0.5};
	GLfloat windowSize[] = {225.0 / 450.0, 540.0 / 600.0};

	GLfloat blockSizes[] = {windowSize[0] * windowPixelHeight / (this->window.getWidth()),
							windowSize[1] * windowPixelWidth / (this->window.getHeight() * aspectRatio)};

	GLfloat minBlockSize = (blockSizes[0] > blockSizes[1]) ? blockSizes[1] : blockSizes[0];

	GLfloat newWindowSize[] = {minBlockSize * this->window.getWidth() / windowPixelHeight,
			 	 	 	 	 aspectRatio * minBlockSize * this->window.getHeight() / windowPixelWidth};

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
	GLfloat tempWindowBlocks[] = {5.0, 5.0};
	GLfloat tempWindowSize[] = {minBlockSize * tempWindowBlocks[0] / windowPixelHeight,
			 	 	 	 	 aspectRatio * minBlockSize * tempWindowBlocks[1] / windowPixelWidth};
	GLfloat tempWindowCentre[] = {352.5 / 450.0, 480.0 / 600.0};

	this->getShaderManager().bindVector2i("uWindowDims", 5, 5);

	this->getShaderManager().bindVector2("uWindowSize", tempWindowSize[0] * 2, tempWindowSize[1] * 2);
	this->getShaderManager().bindVector2("uWindowPos", 2 * tempWindowCentre[0] - 1 - tempWindowSize[0],
														2 * tempWindowCentre[1] - 1 - tempWindowSize[1]);

	this->nextPiece.RenderPiece();

	return true;
}

bool LevelInstance::renderText(void) {
	piecesSetText.Render();
	rowsClearedText.Render();
	playerPointsText.Render();
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

bool LevelInstance::implementUserInput(const UserInputStruct &userInput) {
	UserInputStruct &input = this->getWindow().getInput();
	bool result = true;

	if (input.onClick.left) {
		TetrisPiece tempPiece = this->currentPiece;
		tempPiece.movePieceLeft();
		this->testImplementPieceMove(tempPiece);
	}

	if (input.onClick.right) {
		TetrisPiece tempPiece = this->currentPiece;
		tempPiece.movePieceRight();
		this->testImplementPieceMove(tempPiece);
	}

	if (input.onClick.up) {
		TetrisPiece tempPiece = this->currentPiece;
		tempPiece.rotateCCW();
		this->testImplementRotation(tempPiece);
	}

	if (input.onClick.down) {
		TetrisPiece tempPiece = this->currentPiece;
		tempPiece.movePieceDown();
		result = this->testImplementPieceMove(tempPiece);
		this->resetMoveTimer();
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
	piecesSetText.updateText("Pieces: " + std::to_string(this->piecesSet));
	rowsClearedText.updateText("Rows: " + std::to_string(this->rowsCleared));
	playerPointsText.updateText("Score: " + std::to_string(this->playerPoints));
}

LevelInstance::~LevelInstance(void) {
	INFO << "Killing the level!" << END;
}
