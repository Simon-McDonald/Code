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

#include "Offset.hpp"

namespace mod {
    struct Point {
        GLfloat x;
        GLfloat y;
        GLfloat rot;
        GLfloat filler;
    };

    class Skeleton: protected WorldManager, protected UtilityManager {
    public:
        Skeleton(std::vector<Node> nodeList);

        Skeleton duplicate(void);

        void setUniform(ShaderManager &shader);

        friend Skeleton& operator+(Skeleton& skeleton, const Node& node);

    private:
        std::vector<Node> nodes;
        buf::dynamicUniformBuffer<GLfloat, 2, 1, 1> flesh;

        std::vector<Point> generateUniformData(void);
    };
};

#endif /* SRC_SKELETON_HPP_ */
