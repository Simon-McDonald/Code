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
 *
 */
class LevelInstanceRisingTide: public LevelInstanceRandomStart {
public:
    LevelInstanceRisingTide(void);

protected:
    void addNewRisingLine(void);
    virtual bool updateTimerEvents(double deltaTime_ms) override;

private:
    static const double minLevelTimer_ms;
    static const double levelTimerDelta_ms;

    double riseTimerDuration_ms;
    double riseTimer_ms;

    void resetLevelTimer(void);
};

#endif /* SRC_LEVELINSTANCERISINGTIDE_HPP_ */
