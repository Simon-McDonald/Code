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

namespace mod {

    struct Node {
        size_t parentIdx;
        float x;
        float y;
        float rot;
    };

    struct Point {
        float x;
        float y;
        float rot;
    };

class Skeleton : protected WorldManager, protected UtilityManager {
public:
    Skeleton();

    void setUniform(void);

private:
    std::vector<Node> nodes;
    buf::dynamicUniformBuffer<GLfloat, 2, 1> flesh;

    std::vector<Point> generateUniformData(void);
};

};

#endif /* SRC_SKELETON_HPP_ */
