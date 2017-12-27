/*
 * Levels.hpp
 *
 *  Created on: Dec 11, 2017
 *      Author: Simon
 */

#ifndef SRC_LEVELS_HPP_
#define SRC_LEVELS_HPP_

#include "InstanceType.hpp"
#include "Instance.hpp"
#include "LevelInstance.hpp"
#include "LevelInstanceInvisPieces.hpp"
#include "LevelInstanceNoWalls.hpp"
#include "LevelInstanceRandomStart.hpp"
#include "LevelInstanceRisingTide.hpp"
#include "MenuInstance.hpp"

/*
 * Mapping from instance enum to a dynamically allocated object inheriting LevelInstance.
 */
Instance* generateInstance(InstanceType type) {
    switch (type) {
    case InstanceType::DEFAULT:
        return new LevelInstance;
    case InstanceType::INVIS_BLOCKS:
        return new LevelInstanceInvisPieces;
    case InstanceType::NO_WALLS:
        return new LevelInstanceNoWalls;
    case InstanceType::RANDOM_CLUTTER:
        return new LevelInstanceRandomStart(8);
    case InstanceType::RISING_LEVEL:
        return new LevelInstanceRisingTide;
    case InstanceType::MENU:
        return new MenuInstance({{InstanceType::DEFAULT, "Standard Game"},
                                 {InstanceType::NO_WALLS, "No Walls"},
                                 {InstanceType::INVIS_BLOCKS, "Invisible Set Blocks"},
                                 {InstanceType::RANDOM_CLUTTER, "Random Start"},
                                 {InstanceType::RISING_LEVEL, "Rising Level"},
                                 {InstanceType::QUIT, "Quit"}});
    case InstanceType::QUIT:
    default:
        std::string errorMessage = "Request for instance from invalid InstanceType!";
        throw std::invalid_argument(errorMessage);
    }
}

#endif /* SRC_LEVELS_HPP_ */
