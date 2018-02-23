/*
 * RenderableActor.hpp
 *
 *  Created on: Jan 3, 2018
 *      Author: Simon
 */

#ifndef SRC_RENDERABLEACTOR_HPP_
#define SRC_RENDERABLEACTOR_HPP_

#include <WorldManager.hpp>
#include <UtilityManager.hpp>

#include "Skeleton.hpp"
#include "Flesh.hpp"

class RenderableActor : protected WorldManager, protected UtilityManager {
public:
    RenderableActor(Config::ConfigHeader);

    void render(ShaderManager &shader);

private:
    Config::ConfigHeader configHeader;

    mod::Flesh flesh;
    mod::Skeleton skeleton;
};

#endif /* SRC_RENDERABLEACTOR_HPP_ */
