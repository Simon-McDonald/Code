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

    std::istream& operator>>(std::istream &is, Node &node);

    struct Point {
        GLfloat x;
        GLfloat y;
        GLfloat rot;
        GLfloat filler;
    };

    class SkeletonOffset: protected WorldManager, protected UtilityManager {
    public:
        SkeletonOffset(std::vector<Node> nodes);

        SkeletonOffset operator*(float factor);

        std::vector<Node>::iterator begin(void);
        std::vector<Node>::iterator end(void);

    private:
        std::vector<Node> nodeOffsets;
    };

    class Skeleton: protected WorldManager, protected UtilityManager {
    public:
        Skeleton(std::vector<Node> nodeList);

        Skeleton duplicate(void);

        void setUniform(ShaderManager &shader);

        Skeleton operator+(SkeletonOffset offset);

    private:
        std::vector<Node> nodes;
        buf::dynamicUniformBuffer<GLfloat, 2, 1, 1> flesh;

        std::vector<Point> generateUniformData(void);
    };

    /*struct SkeletonGroup {
        Skeleton baseSkeleton;
        std::vector<SkeletonOffset> offsetList;
    };*/
};

#endif /* SRC_SKELETON_HPP_ */
