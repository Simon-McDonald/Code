/*
 * Skeleton.hpp
 *
 *  Created on: Feb 12, 2018
 *      Author: Simon
 */

#ifndef SRC_SKELETON_HPP_
#define SRC_SKELETON_HPP_

#include <WorldManager.hpp>
#include <UtilityManager.hpp>

#include "glBuffer.hpp"

namespace mod {

    struct Node {
        size_t parentIdx;
        GLfloat x;
        GLfloat y;
        GLfloat rot;
    };

    struct Point {
        GLfloat x;
        GLfloat y;
        GLfloat rot;
        GLfloat filler;
    };

class Skeleton : protected WorldManager, protected UtilityManager {
public:
    Skeleton();

    void setUniform(ShaderManager &shader);

private:
    std::vector<Node> nodes;
    buf::dynamicUniformBuffer<GLfloat, 2, 1, 1> flesh;

    std::vector<Point> generateUniformData(void);
};

};

#endif /* SRC_SKELETON_HPP_ */
