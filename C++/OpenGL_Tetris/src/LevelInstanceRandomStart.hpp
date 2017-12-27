/*
 * LevelInstanceRandomStart.hpp
 *
 *  Created on: Dec 17, 2017
 *      Author: Simon
 */

#ifndef SRC_LEVELINSTANCERANDOMSTART_HPP_
#define SRC_LEVELINSTANCERANDOMSTART_HPP_

#include <random>

#include "LevelInstance.hpp"

/*
 * Start the level with a random assortment of blocks covering the bottom half of the grid.
 */
class LevelInstanceRandomStart : public LevelInstance {
public:
    /*
     * Call the super constructor and add the random blocks.
     */
    LevelInstanceRandomStart(size_t rowsToFill);

protected:
    /*
     * Generate a row of random blocks. Offset is the starting index of the line to insert.
     */
    std::vector<GLubyte> generateRandomBlocksRow(GLubyte width, GLubyte offset);

private:
    std::default_random_engine generator;
    std::uniform_real_distribution<float> randomGen;
};

#endif /* SRC_LEVELINSTANCERANDOMSTART_HPP_ */
