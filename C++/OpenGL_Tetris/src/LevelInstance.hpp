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
 *  - No boundaries on either side----------------------
 *  - Random assortment of pieces at beginning----------
 *  - Incomplete rows periodically added from bottom
 *  - Odd shaped pieces
 */
class LevelInstance : public Instance {
public:
	LevelInstance(void);

	bool update(double deltaTime_ms, const UserInputStruct & userInput);

	virtual void render(void);
	virtual void renderText(void);
	virtual void renderBackground(void);

    virtual InstanceType endState(void);

	virtual ~LevelInstance(void);

protected:
	GridWindow window;
	TetrisPiece currentPiece;
	TetrisPiece nextPiece;

	virtual bool updateTimerEvents(double deltaTime_ms);
    bool moveCurrentPieceDown(void);
    bool generateNextPiece(void);
    bool lockCurrentPiece(void);
    virtual bool testPieceMove(TetrisPiece &piece) const;

	GLfloat calculateMinBlockSize(void);

    std::pair<GLfloat, GLfloat> getAuxiliaryWindowDims(GLfloat blockSize);
    std::pair<GLfloat, GLfloat> getAuxiliaryWindowCentre(void);
    std::pair<GLfloat, GLfloat> getMainWindowDims(GLfloat blockSize);
    std::pair<GLfloat, GLfloat> getMainWindowCentre(void);
    std::pair<GLfloat, GLfloat> getTextWindowDims(void);
    std::pair<GLfloat, GLfloat> getTextWindowCentre(void);

	GLfloat calculateWindowWidth(size_t blockWidth, GLfloat blockSize);
	GLfloat calculateWindowHeight(size_t blockHeight, GLfloat blockSize);

	void setupCommonUniforms(void);
	void setMainWindowUniforms(GLfloat blockSize);
	void setAuxiliaryWindowUniforms(GLfloat blockSize);

	virtual void renderMainWindow(GLfloat blockSize);
	void renderAuxiliaryWindow(GLfloat blockSize);

private:
	static const double minTimer_ms;
	static const float stringStartX;
	static const double pieceMoveDelta_ms;
	static const float aspectRatio;
	static const double animationDelta_ms;

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

	bool gameEnded;
	double animationTimer_ms;
	double animationIndex;

	void startGameEndedAnimation(void);
	bool updateEndAnimation(double deltaTime_ms);
	void resetMoveTimer(void);

	bool testImplementPieceMove(TetrisPiece &piece);
	bool testImplementRotation(TetrisPiece &piece);

	bool implementUserInput(const UserInputStruct &userInput, double deltaTime_ms);

	unsigned pointsForRow(size_t numRowsCleared);

	void resetOutputStrings(void);
};

#endif /* LEVELINSTANCE_HPP_ */
