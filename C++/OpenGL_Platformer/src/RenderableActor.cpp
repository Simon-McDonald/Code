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
 * Random helper, get scaling factor that varies from 0 -> 1 -> 0 ...
 */
float getFactor(float delta_s) {
    const int scale = 1;
    int tempDelta = delta_s;
    tempDelta -= (tempDelta % scale);

    float tempFloat = delta_s - tempDelta;
    tempFloat /= scale;
    tempFloat *= 2.0;
    tempFloat -= 1.0;
    tempFloat = fabs(tempFloat);

    return tempFloat;
}

RenderableActor RenderableActor::generateInstance(std::string fileName) {
    std::ifstream input("models/testModel.txt");

    std::vector<mod::Node> nodes;
    std::vector<mod::Area> surfaces;
    std::string texture;
    std::map<std::string, std::vector<mod::Node>> offsetLists;

    readRenderableActorObjects(input,
                               nodes,
                               surfaces,
                               texture,
                               offsetLists);

    RenderableActor actor(nodes, surfaces, texture, offsetLists);
    return actor;
}

RenderableActor::RenderableActor(
    std::vector<mod::Node> &nodes,
    std::vector<mod::Area> &surfaces,
    std::string &texture,
    std::map<std::string, std::vector<mod::Node>> &offsetLists) :
        flesh(surfaces, texture),
        skeleton(nodes),
        tempFlipper(false),
        flag(false),
        prevFactor(0.0) {

    for (auto &temp : offsetLists) {
        this->offsetList.emplace(temp.first, temp.second);
    }
}

void RenderableActor::render(ShaderManager &shader, float delta_s) {
    float factor = getFactor(delta_s);

    INFO << "Factor = " << factor << ", prevFactor = " << prevFactor << ", Flag = " << flag << ", TempFlipper = " << tempFlipper << END;

    if (prevFactor > factor) {
        flag = true;
    }

    if (prevFactor < factor && flag) {
        tempFlipper = !tempFlipper;
        flag = false;
    }

    prevFactor = factor;

    std::string state = (tempFlipper) ? "state1" : "state2";

    mod::Skeleton bla = this->skeleton + this->offsetList.at(state) * factor;

    bla.setUniform(shader);

    this->flesh.render(shader);
}

bool getHeader(std::istream& is, std::string &header) {
    for (char temp; is >> temp;) {
        //std::cout << ""
        if (temp == '[') {
            std::getline(is, header, ']');

            if (is.eof()) {
                // TODO: Is an error
            } else {
                is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return true;
            }
        }
    }
    return false;
}

std::istream& readRenderableActorObjects(std::istream& is,
                                         std::vector<mod::Node> &nodes,
                                         std::vector<mod::Area> &surfaces,
                                         std::string &texture,
                                         std::map<std::string, std::vector<mod::Node>> &offsetLists) {

    for (std::string header; getHeader(is, header); ) {

        std::cout << "Header: " << header << std::endl;

        if (!header.compare("skeleton")) {
            for (mod::Node node; (is.peek() != '\n') && (is >> node);) {
                nodes.push_back(node);
            }

        } else if (!header.compare("flesh")) {
            for (mod::Area area; (is.peek() != '\n') && (is >> area);) {
                surfaces.push_back(area);
            }

        } else if (!header.compare("skin")) {
            is >> texture;

        } else /* are skeleton offsets */ {
            std::vector<mod::Node> temp;
            for (mod::Node node; (is.peek() != '\n') && (is >> node);) {
                temp.push_back(node);
            }

            offsetLists.emplace(header, temp);
        }
    }

    return is;
}
