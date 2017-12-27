/*
 * LevelInstanceRisingTide.cpp
 *
 *  Created on: Dec 21, 2017
 *      Author: Simon
 */

#include "LevelInstanceRisingTide.hpp"

const double LevelInstanceRisingTide::minLevelTimer_ms = 2000.0;
const double LevelInstanceRisingTide::levelTimerDelta_ms = 100.0;

LevelInstanceRisingTide::LevelInstanceRisingTide(void) :
        LevelInstanceRandomStart(0), riseTimerDuration_ms(5000), riseTimer_ms(riseTimerDuration_ms) {}

void LevelInstanceRisingTide::resetLevelTimer(void) {
    this->riseTimer_ms = this->riseTimerDuration_ms;
}

bool LevelInstanceRisingTide::updateTimerEvents(double deltaTime_ms) {
    bool temp = LevelInstanceRandomStart::updateTimerEvents(deltaTime_ms);

    auto bla = this->getWindow().getInput();
    if (bla.onClick.enter) {
        this->riseTimer_ms = -1;
    }

    this->riseTimer_ms -= deltaTime_ms;

    if (temp && this->riseTimer_ms <= 0) {
        this->riseTimerDuration_ms -= LevelInstanceRisingTide::levelTimerDelta_ms;
        if (this->riseTimerDuration_ms < LevelInstanceRisingTide::minLevelTimer_ms) {
            this->riseTimerDuration_ms = LevelInstanceRisingTide::minLevelTimer_ms;
        }

        TetrisPiece tempPiece = this->currentPiece;
        tempPiece.movePieceDown();

        if (!LevelInstance::testPieceMove(tempPiece)) {
            bool res = LevelInstanceRandomStart::updateTimerEvents(1e10);
            if (!res) {
                return false;
            }
        }

        this->addNewRisingLine();

        this->resetLevelTimer();
    }

    return temp;
}

void LevelInstanceRisingTide::addNewRisingLine(void) {
    std::vector<GLubyte> tempRandomRow = this->generateRandomBlocksRow(
        this->window.getWidth(), 0);

    this->window.insertIntoBottom(1, {150, 150, 150}, tempRandomRow);
}
