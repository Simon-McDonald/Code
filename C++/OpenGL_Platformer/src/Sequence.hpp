/*
 * Sequence.hpp
 *
 *  Created on: Apr 3, 2018
 *      Author: Simon
 */

#ifndef SRC_SEQUENCE_HPP_
#define SRC_SEQUENCE_HPP_

#include <vector>
#include <map>

#include <WorldManager.hpp>
#include <UtilityManager.hpp>

#include "Offset.hpp"

namespace mod {
    /*
     * Pairing of the time at which exact Offset is achieved in the animation sequence.
     */
    struct SequencePoint {
        float time_s;
        Offset offset;
    };

    /*
     * Basic data for construction.
     */
    struct SequenceDefinition {
        std::vector<std::string> offsetList;
        std::vector<float> timeStamps_s;
    };

    std::istream& operator>>(std::istream& is, SequenceDefinition& seqPt);

    class Sequence;

    /*
     * Iterator to loop through the 'Offset' at any point in a sequence, specifically within
     * the transition between two defined Offset instances. Tracks with iterators to the two
     * Offset instances either side of the point in time. Because an Offset can be sparse, must
     * track iterators in both Offsets.
     */
    class SequenceIterator : protected WorldManager, protected UtilityManager {
    public:
        SequenceIterator(void) noexcept;
        SequenceIterator(
            std::array<std::pair<Offset::iterator, Offset::iterator>, 2>& offsetItrs,
            Sequence* animation);

        SequenceIterator& operator++(void);
        Node& operator*(void);
        bool operator!=(SequenceIterator otherItr);

    private:
        std::array<std::pair<Offset::iterator, Offset::iterator>, 2> itrList;
        Sequence *sequence{nullptr};
        Node tempNode{0};

        size_t getMinParent(void);
        bool isAtEnd(const std::pair<Offset::iterator, Offset::iterator>& itrPair);
    };

    /*
     * Defines a time Sequence of transitions between a finite number of Offset states. If
     * an Offset is not specified at time 0s then the default of no offset will be used.
     */
    class Sequence : protected WorldManager, protected UtilityManager {
    public:
        friend class SequenceIterator;

        using iterator = SequenceIterator;

        Sequence(void) noexcept;
        Sequence(const std::vector<std::string> &offsetList,
                 const std::vector<float> &offsetTimeStamps_s,
                 const std::map<std::string, Offset> &offsetMap);

        float getDuration(void) const noexcept;
        float getScale(void) const noexcept;
        void reset(void) noexcept;
        void update(float deltaTime_s) noexcept;
        bool isComplete(void) const noexcept;

        iterator begin(void);
        iterator end(void);

    protected:
        std::vector<SequencePoint> sequences;
        float duration_s{0.0f};
        float currentTime_s{0.0f};
        size_t currentIndex{0u};

        size_t getIdx(float time_s) const;
    };
};

#endif /* SRC_SEQUENCE_HPP_ */
