/*
 * Flesh.cpp
 *
 *  Created on: Feb 19, 2018
 *      Author: Simon
 */

#ifndef SRC_FLESH_HPP_
#define SRC_FLESH_HPP_

#include <WorldManager.hpp>
#include <UtilityManager.hpp>

#include "texBuffer.hpp"
#include "glBuffer.hpp"

namespace mod {

class Flesh : protected WorldManager, protected UtilityManager {
public:
    Flesh(Config::ConfigHeader config);

    void render(ShaderManager &shader);

private:
    buf::texBuffer texture;
    buf::dynamicArrayBuffer<GLfloat, 2, 2> data;

    void setupLayouts(ShaderManager &shader);
};

};

#endif /* SRC_FLESH_HPP_ */
