/*
 * Skeleton.cpp
 *
 *  Created on: Feb 12, 2018
 *      Author: Simon
 */

#include "skeleton.hpp"

namespace mod {

    Skeleton::Skeleton() {
        nodes.push_back({-1, 0.0, 0.1, 0.0});
        nodes.push_back({0, 0.0, 0.1, 0.0});
        nodes.push_back({1, 0.0, 0.1, 0.0});
    }

    void Skeleton::setUniform(void) {
        auto list = this->generateUniformData();

        flesh.resetBuffer(list.size(), sizeof(Point) / sizeof(float), &list[0].x);

        //bind/enable buffer
        //flesh...
    }

    std::vector<Point> Skeleton::generateUniformData(void) {
        std::vector<Point> list;
        list.resize(this->nodes.size());

        list.push_back({this->nodes[0].x, this->nodes[0].y, this->nodes[0].rot});

        for (size_t idx = 1; idx < this->nodes.size(); ++idx) {
            auto parId = this->nodes[idx].parentIdx;

            list.push_back({
                this->nodes[parId].x + this->nodes[idx].x,
                this->nodes[parId].y + this->nodes[idx].y,
                this->nodes[parId].rot + this->nodes[idx].rot});
        }

        return list;
    }


};
