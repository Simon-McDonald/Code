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
    static RenderableActor generateInstance(std::string fileName);

    RenderableActor(
        std::vector<mod::Node> &nodes,
        std::vector<mod::Area> &surfaces,
        std::string &texture,
        std::map<std::string, std::vector<mod::Node>> &offsetLists);

    void render(ShaderManager &shader, float delta_s);

private:
    Config::ConfigHeader configHeader;

    mod::Flesh flesh;
    mod::Skeleton skeleton;

    std::map<std::string, mod::SkeletonOffset> offsetList;

    bool tempFlipper;
    bool flag;
    float prevFactor;
};

std::istream& readRenderableActorObjects(std::istream& is,
                                         std::vector<mod::Node> &nodes,
                                         std::vector<mod::Area> &surfaces,
                                         std::string &texture,
                                         std::map<std::string, std::vector<mod::Node>> &offsetLists);

#endif /* SRC_RENDERABLEACTOR_HPP_ */
