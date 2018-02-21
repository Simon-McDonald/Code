/*
 * Skeleton.cpp
 *
 *  Created on: Feb 12, 2018
 *      Author: Simon
 */

#include "skeleton.hpp"

#include <CheckErrors.h>

namespace mod {

    Skeleton::Skeleton() {
        nodes.push_back({-1u, 0.2, 0.2, 0.0});
        nodes.push_back({0, 0.2, 0.2, 45.0});
        nodes.push_back({1, 0.2, 0.2, 45.0});
        nodes.push_back({2, 0.2, 0.2, 45.0});
        //nodes.push_back({1, 0.0, 0.1, 0.0});
    }

    void Skeleton::setUniform(ShaderManager &shader) {
        auto list = this->generateUniformData();

        flesh.resetBuffer(list.size(), &list[0].x);

        GLuint bla = glGetUniformBlockIndex(shader.getShaderProgram(), "NodeList");
        //GLuint bla2 = glGetUniformLocation(shader.getShaderProgram(), "node.coord");

        INFO << "BLA: " << (bla) << END;
        glUniformBlockBinding(shader.getShaderProgram(), bla, 3);
        glBindBufferBase(GL_UNIFORM_BUFFER, 3, flesh.getId());


        CHECKERRORS();
        //bind/enable buffer
        //flesh...
    }

    std::vector<Point> Skeleton::generateUniformData(void) {
        std::vector<Point> list;
        list.reserve(this->nodes.size());

        list.push_back({this->nodes[0].x, this->nodes[0].y, this->nodes[0].rot});

        for (size_t idx = 1; idx < this->nodes.size(); ++idx) {
            auto parId = this->nodes[idx].parentIdx;
            INFO << "AAA: " << parId << END;

            list.push_back({
                list[parId].x + this->nodes[idx].x,
                list[parId].y + this->nodes[idx].y,
                list[parId].rot + this->nodes[idx].rot});
        }

        INFO << "List: " << END;
        for (size_t idx = 0; idx < list.size(); ++idx) {
            INFO << "[" << idx << "]: (" << list[idx].x << ", " << list[idx].y << "), rot: " << list[idx].rot << END;
        }

        return list;
    }


};
