/*
 * Offset.hpp
 *
 *  Created on: Apr 3, 2018
 *      Author: Simon
 */

#ifndef SRC_OFFSET_HPP_
#define SRC_OFFSET_HPP_

#include <WorldManager.hpp>
#include <UtilityManager.hpp>

#include <cstdlib>

namespace mod {
    struct Node {
        size_t parentIdx;
        GLfloat x;
        GLfloat y;
        GLfloat rot;
    };

    /*
     * Format of Node, "<parent index> <x offset> <y offset> <angle offset>"
     */
    std::istream& operator>>(std::istream &is, Node &node);
    std::ostream& operator<<(std::ostream &os, const Node &node);

    Node operator+(const Node& base, const Node& addValue);
    Node& operator+=(Node& base, const Node& addValue);

    Node operator*(const Node &base, float scale);
    Node operator*(float scale, const Node &base);

    class Offset : protected WorldManager, protected UtilityManager {
    private:
        std::vector<Node> nodeOffsets;

    public:
        using iterator = decltype(nodeOffsets)::iterator;

        Offset(void);
        Offset(std::vector<Node>& nodes);

        iterator begin(void);
        iterator end(void);
    };

    /*
     * Format of Offset "<node1>\n<node2>...<nodeN-1>\n\n"
     */
    std::istream& operator>>(std::istream &is, Offset &offset);
    std::ostream& operator<<(std::ostream &os, Offset &offset);
};

#endif /* SRC_OFFSET_HPP_ */
