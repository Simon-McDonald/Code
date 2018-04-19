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
#include "Offset.hpp"
#include "Sequence.hpp"
#include "Animation.hpp"

class RenderableActor : protected WorldManager, protected UtilityManager {
public:
    static RenderableActor generateInstance(std::string fileName);

    RenderableActor(
        std::vector<mod::Node> &nodes,
        std::vector<mod::Area> &surfaces,
        std::string &texture,
        std::map<std::string, std::vector<mod::Node>> &offsetLists,
        std::map<std::string, mod::SequenceDefinition> &sequenceDefs,
        std::map<std::string, mod::AnimationDefinition> &animationsList);

    void update(float delta_s, const UserInputStruct& input);
    void render(ShaderManager &shader, float delta_s);

private:
    Config::ConfigHeader configHeader;

    mod::Flesh flesh;
    mod::Skeleton skeleton;

    std::map<std::string, mod::Animation> animations;

    bool tempFlipper;
    bool flag;
    float prevFactor;
};

#endif /* SRC_RENDERABLEACTOR_HPP_ */
