/*
 * Flesh.cpp
 *
 *  Created on: Feb 19, 2018
 *      Author: Simon
 */

#include <limits>

#include "Flesh.hpp"

namespace mod {

    std::istream& operator>>(std::istream &is, Area &area) {
        int temp;

        is >> temp >> area.x >> area.y >> area.w >> area.h >> area.tx >> area.ty >> area.tw >> area.th;
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        return is;
    }

Flesh::Flesh(std::vector<Area> areas, std::string skinTexture) :
    texture(skinTexture, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE) {

    this->data.resetBuffer(areas.size(), &areas[0].x);
}

void Flesh::render(ShaderManager &shader) {
    setupLayouts(shader);

    CHECKERRORS();

    manageRenderArray(data, GL_POINTS, 1, 2, 3, 4);

    CHECKERRORS();
}

void Flesh::setupLayouts(ShaderManager &shader) {
    this->texture.bindTexture(shader, "gActorSkin");
}

};
