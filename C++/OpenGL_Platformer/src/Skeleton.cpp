/*
 * Skeleton.cpp
 *
 *  Created on: Feb 12, 2018
 *      Author: Simon
 */

#include <cmath>
#include <limits>

#include "skeleton.hpp"

#include <CheckErrors.h>

namespace mod {

    std::istream& operator>>(std::istream &is, Node &node) {
        is >> node.parentIdx >> node.x >> node.y >> node.rot;
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        return is;
    }

    SkeletonOffset::SkeletonOffset(std::vector<Node> nodes) : nodeOffsets(nodes) {}

    SkeletonOffset SkeletonOffset::operator*(float factor) {
        SkeletonOffset tempOffset = *this;

        for (auto &node : this->nodeOffsets) {
            node.x *= factor;
            node.y *= factor;
            node.rot *= factor;
        }

        std::swap(*this, tempOffset);
        return tempOffset;
    }

    std::vector<Node>::iterator SkeletonOffset::begin(void) {
        return this->nodeOffsets.begin();
    }

    std::vector<Node>::iterator SkeletonOffset::end(void) {
        return this->nodeOffsets.end();
    }

    Skeleton::Skeleton(const std::vector<Node> nodeList) {
        this->nodes = nodeList;
    }

    void Skeleton::setUniform(ShaderManager &shader) {
        auto list = this->generateUniformData();

        flesh.resetBuffer(list.size(), &list[0].x);
        bindUniformBuffer(flesh, shader.getShaderProgram(), 3, "NodeList");

        CHECKERRORS();
    }

    std::vector<Point> Skeleton::generateUniformData(void) {
        std::vector<Point> list;
        list.reserve(this->nodes.size());

        list.push_back({this->nodes[0].x, this->nodes[0].y, this->nodes[0].rot});

        for (size_t idx = 1; idx < this->nodes.size(); ++idx) {
            auto parId = this->nodes[idx].parentIdx;

            float theta = -list[parId].rot * M_PI / 180.0;
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);

            float x = this->nodes[idx].x;
            float y = this->nodes[idx].y;

            list.push_back({
                list[parId].x + x * cosTheta - y * sinTheta,
                list[parId].y + x * sinTheta + y * cosTheta,
                list[parId].rot + this->nodes[idx].rot});
        }

        return list;
    }

    Skeleton Skeleton::duplicate(void) {
        Skeleton skeleton(this->nodes);
        return skeleton;
    }

    Skeleton Skeleton::operator+(SkeletonOffset offsets) {
        Skeleton temp = this->duplicate();

        for (const auto& nodeOffset : offsets) {
            temp.nodes.at(nodeOffset.parentIdx).x += nodeOffset.x;
            temp.nodes.at(nodeOffset.parentIdx).y += nodeOffset.y;
            temp.nodes.at(nodeOffset.parentIdx).rot += nodeOffset.rot;
        }

        return temp;
    }
};
