/*
 * RenderableActor.cpp
 *
 *  Created on: Jan 3, 2018
 *      Author: Simon
 */

#include <limits>
#include <cmath>
#include <utility>

#include "RenderableActor.hpp"

/*
 * Find next heading, identified as being wrapped by [...].
 */
bool getHeader(std::istream& is, std::string &header) {
    for (char temp; is >> temp;) {
        if (temp == '[') {
            std::getline(is, header, ']');

            if (is.eof()) {
                break;
            } else {
                is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return true;
            }
        }
    }
    return false;
}

/*
 * Read a file that contains animation definitions for a model.
 */
bool readRenderableActorObjects(std::istream& is,
    std::vector<mod::Node> &nodes, // skeleton
    std::vector<mod::Area> &surfaces, // flesh
    std::string &texture, // skin
    std::map<std::string, std::vector<mod::Node>> &offsetLists, // offsets (states)
    std::map<std::string, mod::SequenceDefinition> &sequenceDefs, // sequences
    std::map<std::string, mod::AnimationDefinition> &animationDefs) { // animations

    for (std::string header; getHeader(is, header); ) {
        if (!header.compare("SKELETON")) {
            for (mod::Node node; (is.peek() != '\n') && (is >> node);) {
                is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                nodes.push_back(node);
            }

        } else if (!header.compare("FLESH")) {
            for (mod::Area area; (is.peek() != '\n') && (is >> area);) {
                surfaces.push_back(area);
                is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } else  {
            std::string nameLine, key, value;
            std::getline(is, nameLine);
            cvt::splitAtDelim(nameLine, '=', key, value);

            if (!header.compare("SKIN")) {
                texture = value;
            } else if (!header.compare("STATE")) {
                std::vector<mod::Node> temp;
                for (mod::Node node; (is.peek() != '\n') && (is >> node);) {
                    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    temp.push_back(node);
                }

                offsetLists.emplace(value, temp);
            } else if (!header.compare("SEQUENCE")) {
                mod::SequenceDefinition seqDef;
                is >> seqDef;
                sequenceDefs.emplace(value, seqDef);

            } else if (!header.compare("ANIMATION")) {
                mod::AnimationDefinition animDef;
                is >> animDef;
                animationDefs.emplace(value, animDef);
            }
        }
    }

    return true;
}

RenderableActor RenderableActor::generateInstance(std::string fileName) {
    std::ifstream input(fileName);

    std::vector<mod::Node> nodes;
    std::vector<mod::Area> surfaces;
    std::string texture;
    std::map<std::string, std::vector<mod::Node>> offsetLists;
    std::map<std::string, mod::SequenceDefinition> sequenceDefs;
    std::map<std::string, mod::AnimationDefinition> animations;

    readRenderableActorObjects(input,
                               nodes,
                               surfaces,
                               texture,
                               offsetLists,
                               sequenceDefs,
                               animations);

    RenderableActor actor(nodes, surfaces, texture, offsetLists, sequenceDefs, animations);
    return actor;
}

RenderableActor::RenderableActor(
    std::vector<mod::Node> &nodes,
    std::vector<mod::Area> &surfaces,
    std::string &texture,
    std::map<std::string, std::vector<mod::Node>> &offsetLists,
    std::map<std::string, mod::SequenceDefinition> &sequenceDefs,
    std::map<std::string, mod::AnimationDefinition> &animationsList) :
        flesh(surfaces, texture),
        skeleton(nodes),
        tempFlipper(false),
        flag(false),
        prevFactor(0.0) {

    std::map<std::string, mod::Offset> offsetMap;
    for (auto &offsetDef : offsetLists) {
        offsetMap.emplace(offsetDef.first, offsetDef.second);
    }

    std::map<std::string, mod::Sequence> sequenceMap;
    for (auto &seqDef : sequenceDefs) {
        sequenceMap.emplace(seqDef.first,
            mod::Sequence{seqDef.second.offsetList, seqDef.second.timeStamps_s, offsetMap});
    }

    for (auto &temp : animationsList) {
        this->animations.emplace(temp.first, mod::Animation{temp.second.inSequenceName,
                                              temp.second.loopSequenceNames,
                                              temp.second.outSequenceNames,
                                              sequenceMap});
    }
}

void RenderableActor::update(float delta_s, const UserInputStruct& input) {
    if (input.onClick.right) {
        this->animations.at("animation1").start();
    } else if (input.onRelease.right) {
        this->animations.at("animation1").flagEnd();
    }

    for (auto &animation : this->animations) {
        animation.second.update(delta_s);
    }
}

void RenderableActor::render(ShaderManager &shader, float delta_s) {
    mod::Skeleton animatedSkeleton = this->skeleton.duplicate();

    for (auto &animation : this->animations) {
        for (const auto& node : animation.second) {
            animatedSkeleton + node;
        }
    }

    animatedSkeleton.setUniform(shader);

    this->flesh.render(shader);
}
