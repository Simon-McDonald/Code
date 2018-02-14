/*
 * RenderableActor.cpp
 *
 *  Created on: Jan 3, 2018
 *      Author: Simon
 */

#include "RenderableActor.hpp"

RenderableActor::RenderableActor(Config::ConfigHeader config) :
    configHeader(config),
    texture(RenderableActor::getConfig().getValue<std::string>(this->configHeader, "texture"), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE) {

    GLfloat dataPoints[] = {0.0, 0.0, 0.5, 0.5};
    this->data.resetBuffer(1, &dataPoints[0]);
}

void RenderableActor::render(ShaderManager &shader) {
    this->texture.bindTexture(shader, "gActorSkin");

    this->data.manageRender(1, 2);
}
