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
 *
 */
class LevelInstanceRandomStart : public LevelInstance {
public:
    LevelInstanceRandomStart(void);

protected:
    std::vector<GLubyte> generateRandomBlocksRow(GLubyte width, GLubyte offset);

private:
    std::default_random_engine generator;
    std::uniform_real_distribution<float> randomGen;
};

#endif /* SRC_LEVELINSTANCERANDOMSTART_HPP_ */
