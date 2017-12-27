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
#include "RenderableText.hpp"
#include "Instance.hpp"

/*
 * Manager class for a level of Tetris. All levels inherit from this class.
 */
class LevelInstance : public Instance {
public:
	LevelInstance(void);

	/*
	 * Overloads for the abstract base class.
	 */
	bool update(double deltaTime_ms, const UserInputStruct & userInput);

	virtual void render(void);
	virtual void renderText(void);
	virtual void renderBackground(void);

    virtual InstanceType endState(void);

	virtual ~LevelInstance(void);

protected:
	/*
	 * Renderable objects.
	 */
	GridWindow window;
	TetrisPiece currentPiece;
	TetrisPiece nextPiece;

	/*
	 * Update game state helper functions.
	 */
	virtual bool updateTimerEvents(double deltaTime_ms);
    bool moveCurrentPieceDown(void);
    bool generateNextPiece(void);
    bool lockCurrentPiece(void);
    virtual bool testPieceMove(TetrisPiece &piece) const;

    /*
     * Find the block size in render space [-1, 1] by [-1, 1].
     */
	GLfloat calculateMinBlockSize(void);

	/*
	 * Get relevant dimensions for rendering.
	 */
    std::pair<GLfloat, GLfloat> getAuxiliaryWindowDims(GLfloat blockSize);
    std::pair<GLfloat, GLfloat> getAuxiliaryWindowCentre(void);
    std::pair<GLfloat, GLfloat> getMainWindowDims(GLfloat blockSize);
    std::pair<GLfloat, GLfloat> getMainWindowCentre(void);
    std::pair<GLfloat, GLfloat> getTextWindowDims(void);
    std::pair<GLfloat, GLfloat> getTextWindowCentre(void);
	GLfloat calculateWindowWidth(size_t blockWidth, GLfloat blockSize);
	GLfloat calculateWindowHeight(size_t blockHeight, GLfloat blockSize);

	/*
	 * Helper functions to prepare rendering program.
	 */
	void setupCommonUniforms(void);
	void setMainWindowUniforms(GLfloat blockSize);
	void setAuxiliaryWindowUniforms(GLfloat blockSize);

	/*
	 * Rendering functions.
	 */
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

	void setCurrentPieceStartLoc(void);

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
