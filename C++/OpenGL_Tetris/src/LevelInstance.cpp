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
const double LevelInstance::animationDelta_ms = 100.0;

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
		rightButtonDuration(0.0),
		gameEnded(false),
	    animationTimer_ms(0.0),
	    animationIndex (0) {

    this->setCurrentPieceStartLoc();
	this->resetOutputStrings();
}

bool LevelInstance::update(double deltaTime_ms, const UserInputStruct &userInput) {
    if (gameEnded) {
        return this->updateEndAnimation(deltaTime_ms);
    }

    bool result = true;

    // Implement user input, if false indicates that input locks piece (i.e. push down but piece can't move down).
    if (!this->implementUserInput(userInput, deltaTime_ms)) {
        this->resetMoveTimer();
        this->lockCurrentPiece();

        if (!this->generateNextPiece()) {
            // game over
            result = false;
        }
    } else if (!this->updateTimerEvents(deltaTime_ms)) {
        result = false;
    }

    if (!result) {
        this->startGameEndedAnimation();
        result = true;
    }

    return result;
}

void LevelInstance::render(void) {
    GLfloat minBlockSize = this->calculateMinBlockSize();
    this->setupCommonUniforms();
    this->renderMainWindow(minBlockSize);
    this->renderAuxiliaryWindow(minBlockSize);
}

void LevelInstance::renderText(void) {
    labelsText.Render();
    valuesText.Render();
}

// TODO maintain single buffer between each rendering call.
void LevelInstance::renderBackground(void) {
    GLfloat blockSize = this->calculateMinBlockSize();

    auto mainWindowCentre = this->getMainWindowCentre();
    auto mainWindowSize = this->getMainWindowDims(blockSize);

    auto auxWindowCentre = this->getAuxiliaryWindowCentre();
    auto auxWindowSize = this->getAuxiliaryWindowDims(blockSize);

    auto textWindowCentre = this->getTextWindowCentre();
    auto textWindowSize = this->getTextWindowDims();

    std::vector<GLfloat> borderPoints = {
        mainWindowCentre.first, mainWindowCentre.second, mainWindowSize.first, mainWindowSize.second,
        auxWindowCentre.first, auxWindowCentre.second, auxWindowSize.first, auxWindowSize.second,
        textWindowCentre.first, textWindowCentre.second, textWindowSize.first, textWindowSize.second};

    DataBuffer<GLfloat, 2, 2> borderBuffer(3, &borderPoints[0]);
    borderBuffer.manageRender(1, 2);
}

InstanceType LevelInstance::endState(void) {
    return InstanceType::MENU;
}

LevelInstance::~LevelInstance(void) {}

bool LevelInstance::updateTimerEvents(double deltaTime_ms) {
    this->currentTimer_ms -= deltaTime_ms;

    if (this->currentTimer_ms <= 0) {
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

bool LevelInstance::generateNextPiece() {
    this->currentPiece = this->nextPiece;
    this->setCurrentPieceStartLoc();

    this->nextPiece = TetrisPiece::generateNextTetrisPiece();

    return this->testPieceMove(this->currentPiece);
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

bool LevelInstance::testPieceMove(TetrisPiece &piece) const {
    std::pair<size_t, size_t> location = piece.getPieceLoc();

    for (size_t heightIndex = 0; heightIndex < piece.getHeight(); ++heightIndex) {
        for (size_t widthIndex = 0; widthIndex < piece.getWidth(); ++widthIndex) {

            if (piece(widthIndex, heightIndex) &&
                (((widthIndex + location.first >= this->window.getWidth()) ||
                  (heightIndex + location.second >= this->window.getHeight())) ||
                this->window(widthIndex + location.first, heightIndex + location.second)))
            {
                return false;
            }
        }
    }

    return true;
}

GLfloat LevelInstance::calculateMinBlockSize(void) {
    GLfloat windowSize[] = {300.0 / 540.0, 540.0 / 600.0};

    GLfloat windowPixelHeight = (float) this->getWindow().getHeight();
    GLfloat windowPixelWidth = (float) this->getWindow().getWidth();

    GLfloat blockSizes[] = {windowSize[0] * windowPixelWidth / (this->window.getWidth()),
                            windowSize[1] * windowPixelHeight /
                                (this->window.getHeight() * LevelInstance::aspectRatio)};

    return (blockSizes[0] > blockSizes[1]) ? blockSizes[1] : blockSizes[0];
}

std::pair<GLfloat, GLfloat> LevelInstance::getAuxiliaryWindowDims(GLfloat blockSize) {
    GLfloat tempWindowBlocks[] = {5.0, 5.0};

    return std::make_pair<GLfloat, GLfloat>(
        this->calculateWindowWidth(tempWindowBlocks[0], blockSize),
        this->calculateWindowHeight(tempWindowBlocks[1], blockSize));
}
std::pair<GLfloat, GLfloat> LevelInstance::getAuxiliaryWindowCentre(void) {
    return std::make_pair<GLfloat, GLfloat>(450 / 540.0, 480.0 / 600.0);
}

std::pair<GLfloat, GLfloat> LevelInstance::getMainWindowDims(GLfloat blockSize) {
    return std::make_pair<GLfloat, GLfloat>(
        this->calculateWindowWidth(this->window.getWidth(), blockSize),
        this->calculateWindowHeight(this->window.getHeight(), blockSize));
}
std::pair<GLfloat, GLfloat> LevelInstance::getMainWindowCentre(void) {
    return std::make_pair<GLfloat, GLfloat>(180.0 / 540.0, 0.5);
}

std::pair<GLfloat, GLfloat> LevelInstance::getTextWindowDims(void) {
    return std::make_pair<GLfloat, GLfloat>(0.32, 0.3);
}
std::pair<GLfloat, GLfloat> LevelInstance::getTextWindowCentre(void) {
    return std::make_pair<GLfloat, GLfloat>(0.82, 0.32);
}

GLfloat LevelInstance::calculateWindowWidth(size_t blockWidth, GLfloat blockSize) {
    return blockSize * blockWidth / this->getWindow().getWidth();
}

GLfloat LevelInstance::calculateWindowHeight(size_t blockHeight, GLfloat blockSize) {
    return LevelInstance::aspectRatio * blockSize  * blockHeight / this->getWindow().getHeight();
}

void LevelInstance::setupCommonUniforms(void) {
    this->getShaderManager().setUniformInt("uGridWidth", this->window.getWidth());
    this->currentPiece.prepShaderTextures();
}

void LevelInstance::setMainWindowUniforms(GLfloat blockSize) {
    auto newWindowSize = this->getMainWindowDims(blockSize);
    auto windowCentre = this->getMainWindowCentre();

    this->getShaderManager().bindVector2("uWindowSize", newWindowSize.first * 2, newWindowSize.second * 2);
    this->getShaderManager().bindVector2("uWindowPos", 2 * windowCentre.first - 1 - newWindowSize.first,
                                                       2 * windowCentre.second - 1 - newWindowSize.second);
}

void LevelInstance::setAuxiliaryWindowUniforms(GLfloat blockSize) {
    auto tempWindowCentre = this->getAuxiliaryWindowCentre();
    auto tempWindowSize = this->getAuxiliaryWindowDims(blockSize);

    this->getShaderManager().bindVector2("uWindowSize", tempWindowSize.first * 2, tempWindowSize.second * 2);
    this->getShaderManager().bindVector2("uWindowPos", 2 * tempWindowCentre.first - 1 - tempWindowSize.first,
                                                       2 * tempWindowCentre.second - 1 - tempWindowSize.second);
}

void LevelInstance::renderMainWindow(GLfloat blockSize) {
    this->window.setGridDimsInShader();
    this->setMainWindowUniforms(blockSize);

    this->getShaderManager().bindVector2i("uPieceOffset", 0, 0);
    this->window.render();

    this->getShaderManager().bindVector2i("uPieceOffset", this->currentPiece.getPieceLoc().first,
                                          this->currentPiece.getPieceLoc().second);
    this->currentPiece.RenderPiece();

}

void LevelInstance::renderAuxiliaryWindow(GLfloat blockSize) {
    this->setAuxiliaryWindowUniforms(blockSize);
    this->getShaderManager().bindVector2i("uWindowDims", 5, 5);
    this->getShaderManager().bindVector2i("uPieceOffset", 0, 0);
    this->nextPiece.RenderPiece();
}

void LevelInstance::setCurrentPieceStartLoc(void) {
    currentPiece.setPieceLoc(4, 15);
}

void LevelInstance::startGameEndedAnimation(void) {
    this->gameEnded = true;
    this->animationTimer_ms = LevelInstance::animationDelta_ms;
    this->animationIndex = 0;
}

bool LevelInstance::updateEndAnimation(double deltaTime_ms) {
    this->animationTimer_ms -= deltaTime_ms;

    if (this->animationTimer_ms <= 0) {
        if (this->animationIndex == this->window.getHeight()) {
            return false;
        }

        this->window.setRowDefault(animationIndex, {150, 150, 150});
        ++this->animationIndex;

        this->animationTimer_ms = LevelInstance::animationDelta_ms;
    }

    return true;
}

void LevelInstance::resetMoveTimer(void) {
    this->currentTimer_ms = this->timerDuration_ms;
}

bool LevelInstance::testImplementPieceMove(TetrisPiece &piece) {
	if (testPieceMove(piece)) {
		this->currentPiece = piece;

		return true;
	}
	return false;
}

bool LevelInstance::testImplementRotation(TetrisPiece &piece) {
    if (this->testImplementPieceMove(piece)) {
        return true;
    }

    piece.movePieceLeft(this->window.getWidth());
    if (this->testImplementPieceMove(piece)) {
        return true;
    }

    piece.movePieceRight(this->window.getWidth());
    piece.movePieceRight(this->window.getWidth());
    if (this->testImplementPieceMove(piece)) {
        return true;
    }

    return false;
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
			tempPiece.movePieceLeft(this->window.getWidth());
			this->testImplementPieceMove(tempPiece);
		} else if (input.onDown.left) {
			leftButtonDuration += deltaTime_ms;
		}

		if (input.onClick.right || (rightButtonDuration > 1.5 * LevelInstance::pieceMoveDelta_ms)) {
			this->rightButtonDuration = 0.0;
			TetrisPiece tempPiece = this->currentPiece;
			tempPiece.movePieceRight(this->window.getWidth());
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

unsigned LevelInstance::pointsForRow(size_t numRowsCleared) {
	return numRowsCleared * numRowsCleared;
}

void LevelInstance::resetOutputStrings(void) {
    valuesText.updateText(std::to_string(this->piecesSet) + "\n" +
        std::to_string(this->rowsCleared) + "\n" + std::to_string(this->playerPoints));
}
