/*
 * Flesh.cpp
 *
 *  Created on: Feb 19, 2018
 *      Author: Simon
 */

#include "Flesh.hpp"

namespace mod {

Flesh::Flesh(Config::ConfigHeader config) :
    texture(Flesh::getConfig().getValue<std::string>(config, "texture"), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE) {

    GLfloat dataPoints[] = {0.0, 0.0, 0.3, 0.3, 0.0, 0.0, 0.2, 0.2, 0.0, 0.0, 0.2, 0.2, 0.0, 0.0, 0.2, 0.2};
    this->data.resetBuffer(sizeof(dataPoints) / sizeof(dataPoints[0] / 4), &dataPoints[0]);
}

void Flesh::render(ShaderManager &shader) {
    setupLayouts(shader);

    this->data.manageRender(1, 2);
}

void Flesh::setupLayouts(ShaderManager &shader) {
    this->texture.bindTexture(shader, "gActorSkin");
}

};
