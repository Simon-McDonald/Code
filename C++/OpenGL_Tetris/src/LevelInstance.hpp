/*
 * LevelInstance.hpp
 *
 *  Created on: Jul 9, 2017
 *      Author: Simon
 */

#ifndef LEVELINSTANCE_HPP_
#define LEVELINSTANCE_HPP_

#include <utility>

#include "GridWindow.hpp"
#include "TetrisPiece.hpp"
#include "TextImage.hpp"
#include "Instance.hpp"

/*
 * Manager class for a level of Tetris.
 * Game variants to add:
 *  - Standard------------------------------------------
 *  - Set pieces are invisible--------------------------
 *  - No boundaries on either side
 *  - Random assortment of pieces at beginning
 *  - Incomplete rows periodically added from bottom
 */
class LevelInstance : public Instance {
public:
	LevelInstance(void);

	bool update(double deltaTime_ms, const UserInputStruct & userInput);

	virtual bool render(void);
	bool renderText(void);
	bool renderBackground(void);

	virtual ~LevelInstance(void);

protected:

	GridWindow window;
	TetrisPiece currentPiece;
	TetrisPiece nextPiece;

private:
	static const double minTimer_ms;
	static const float stringStartX;
	static const double pieceMoveDelta_ms;
	static const float aspectRatio;

	double timerDuration_ms;
	double currentTimer_ms;
	double timerChangeScaling;

	TextImage textImageInst;
	RenderableText labelsText;
	RenderableText valuesText;

	unsigned piecesSet;
	unsigned rowsCleared;
	unsigned playerPoints;

	double downButtonDuration;
	double leftButtonDuration;
	double rightButtonDuration;

	void resetMoveTimer(void);

	bool testPieceMove(TetrisPiece &piece);

	bool testImplementPieceMove(TetrisPiece &piece);
	bool testImplementRotation(TetrisPiece &piece);

	bool implementUserInput(const UserInputStruct &userInput, double deltaTime_ms);

	bool moveCurrentPieceDown(void);

	bool lockCurrentPiece(void);

	unsigned pointsForRow(size_t numRowsCleared);

	void resetOutputStrings(void);

	bool generateNextPiece(void);
};

#endif /* LEVELINSTANCE_HPP_ */
