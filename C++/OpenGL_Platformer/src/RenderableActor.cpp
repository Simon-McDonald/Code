/*
 * RenderableActor.cpp
 *
 *  Created on: Jan 3, 2018
 *      Author: Simon
 */

#include "RenderableActor.hpp"

RenderableActor::RenderableActor(Config::ConfigHeader config) :
    configHeader(config),
    flesh(config) {
}

void RenderableActor::render(ShaderManager &shader) {
    this->skeleton.setUniform(shader);
    this->flesh.render(shader);
}
