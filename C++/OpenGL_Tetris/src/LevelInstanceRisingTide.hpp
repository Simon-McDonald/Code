/*
 * LevelInstanceRisingTide.hpp
 *
 *  Created on: Dec 21, 2017
 *      Author: Simon
 */

#ifndef SRC_LEVELINSTANCERISINGTIDE_HPP_
#define SRC_LEVELINSTANCERISINGTIDE_HPP_

#include "LevelInstanceRandomStart.hpp"

/*
 * Periodically add a new row to the bottom of the screen. Inherits from the RandomStart level.
 * TODO don't have blocks at start
 */
class LevelInstanceRisingTide: public LevelInstanceRandomStart {
public:
    LevelInstanceRisingTide(void);

protected:
    /*
     * Add the new line.
     */
    void addNewRisingLine(void);

    /*
     * Tack on the functionality to manage the timing of adding new rows.
     */
    virtual bool updateTimerEvents(double deltaTime_ms) override;

private:
    static const double minLevelTimer_ms;
    static const double levelTimerDelta_ms;

    double riseTimerDuration_ms;
    double riseTimer_ms;

    void resetLevelTimer(void);
};

#endif /* SRC_LEVELINSTANCERISINGTIDE_HPP_ */
