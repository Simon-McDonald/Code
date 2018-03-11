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

#include "glBuffer.hpp"
#include "texBuffer.hpp"

namespace mod {

    struct Area {
        float x;
        float y;
        float w;
        float h;

        float tx;
        float ty;
        float tw;
        float th;
    };

    std::istream& operator>>(std::istream &is, Area &area);

class Flesh : protected WorldManager, protected UtilityManager {
public:
    Flesh(std::vector<Area> areas, std::string skinTexture);

    void extend(Area &area);

    void render(ShaderManager &shader);

private:
    buf::texBuffer texture;

    std::vector<Area> areas;
    buf::dynamicArrayBuffer<GLfloat, 2, 2, 2, 2> data;

    void setupLayouts(ShaderManager &shader);
};

};

#endif /* SRC_FLESH_HPP_ */
