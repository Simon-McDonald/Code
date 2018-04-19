/*
 * Offset.cpp
 *
 *  Created on: Apr 3, 2018
 *      Author: Simon
 */

#include <limits>

#include "Offset.hpp"

namespace mod {

    std::istream& operator>>(std::istream &is, Node &node) {
        is >> node.parentIdx >> node.x >> node.y >> node.rot;
        return is;
    }

    std::ostream& operator<<(std::ostream &os, const Node &node) {
        os << node.parentIdx << " " << node.x << " " << node.y << " " << node.rot;;
        return os;
    }

    Node operator+(const Node& base, const Node& addValue) {
        Node baseCpy = base;

        baseCpy.rot += addValue.rot;
        baseCpy.x += addValue.x;
        baseCpy.y += addValue.y;

        return baseCpy;
    }

    Node& operator+=(Node& base, const Node& addValue) {
        base.rot += addValue.rot;
        base.x += addValue.x;
        base.y += addValue.y;

        return base;
    }

    Node operator*(const Node& base, float scale) {
        return Node{base.parentIdx, base.x * scale, base.y * scale, base.rot * scale};
    }

    Node operator*(float scale, const Node& base) {
        return base * scale;
    }

    Offset::Offset(std::vector<Node>& nodes) : nodeOffsets(nodes) {
        for (const auto& node : nodeOffsets) {
            if (node.parentIdx < 0) {
                std::stringstream ss;
                ss << "Malformed Node in Offset: (" << node << ")";
                throw std::invalid_argument(ss.str());
            }
        }
    }
    Offset::Offset(void) {}

    Offset::iterator Offset::begin(void) {
        return this->nodeOffsets.begin();
    }

    Offset::iterator Offset::end(void) {
        return this->nodeOffsets.end();
    }

    std::istream& operator>>(std::istream &is, Offset &offset) {
        std::vector<Node> nodeList;

        for (Node node; (is.peek() != '\n') && (is >> node);) {
            nodeList.push_back(node);
        }

        offset = Offset(nodeList);
        return is;
    }

    std::ostream& operator<<(std::ostream &os, Offset &offset) {
        for (const auto& node : offset) {
            os << node << "\n";
        }
        os << "\n";

        return os;
    }
};
