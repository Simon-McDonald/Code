/*
 * LevelInstance.hpp
 *
 *  Created on: Jul 9, 2017
 *      Author: Simon
 */

#ifndef LEVELINSTANCE_HPP_
#define LEVELINSTANCE_HPP_

#include <utility>

#include "UtilityManager.hpp"
#include "GridWindow.hpp"
#include "TetrisPiece.hpp"
#include "TextImage.hpp"

/*
 * Manager class for a level of Tetris.
 * Game variants to add:
 *  - Standard
 *  - Set pieces are invisible
 *  - No boundaries on either side
 *  - Random assortment of pieces at beginning
 *  - Incomplete rows periodically added from bottom
 */
class LevelInstance : protected UtilityManager, protected WorldManager {
public:
	LevelInstance(void);

	bool update(double deltaTime_ms, const UserInputStruct & userInput);

	bool render(void);
	bool renderText(void);
	bool renderBackground(void);

	~LevelInstance(void);

private:
	static const double minTimer_ms;
	static const float stringStartX;

	GridWindow window;
	TetrisPiece currentPiece;
	TetrisPiece nextPiece;
	double timerDuration_ms;
	double currentTimer_ms;
	double timerChangeScaling;

	TextImage textImageInst;
	RenderableText piecesSetText;
	RenderableText rowsClearedText;
	RenderableText playerPointsText;

	unsigned piecesSet;
	unsigned rowsCleared;
	unsigned playerPoints;



	void resetMoveTimer(void);

	bool testPieceMove(TetrisPiece &piece);

	bool testImplementPieceMove(TetrisPiece &piece);
	bool testImplementRotation(TetrisPiece &piece);

	bool implementUserInput(const UserInputStruct &userInput);

	bool moveCurrentPieceDown(void);

	bool lockCurrentPiece(void);

	unsigned pointsForRow(size_t numRowsCleared);

	void resetOutputStrings(void);

	bool generateNextPiece(void);
};

#endif /* LEVELINSTANCE_HPP_ */
