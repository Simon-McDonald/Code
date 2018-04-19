/*
 * Sequence.cpp
 *
 *  Created on: Apr 3, 2018
 *      Author: Simon
 */

#include <cassert>

#include "Sequence.hpp"

namespace mod {
    std::istream& operator>>(std::istream& is, SequenceDefinition& seqDef) {
        for (std::string line;
             (is.peek() != '\n') && std::getline(is, line); ) {

            std::string title, listString;
            cvt::splitAtDelim(line, '=', title, listString);

            if (title == "state_list") {
                seqDef.offsetList = cvt::splitToList<std::string>(listString);
            } else if (title == "time_stamps_s") {
                seqDef.timeStamps_s = cvt::splitToList<float>(listString);
            }
        }

        return is;
    }

    SequenceIterator::SequenceIterator(void) noexcept {}

    SequenceIterator::SequenceIterator(
        std::array<std::pair<Offset::iterator, Offset::iterator>, 2>& offsetItrs,
        Sequence* sequence) :
            itrList(offsetItrs), sequence(sequence) {}

    SequenceIterator& SequenceIterator::operator++(void) {
        size_t minParentIdx = this->getMinParent();

        if (minParentIdx == -1u) {
            std::stringstream ss;
            ss << "Attempting to increment iterator at container end.";
            throw std::out_of_range(ss.str());
        }

        for (auto& itrPair : this->itrList) {
            if (this->isAtEnd(itrPair)) {
                continue;
            }

            if (itrPair.first->parentIdx == minParentIdx) {
                ++itrPair.first;
            }
        }

        return *this;
    }

    Node& SequenceIterator::operator*(void) {
        size_t minParentIdx = this->getMinParent();

        if (minParentIdx == -1u) {
            std::stringstream ss;
            ss << "Attempting to dereference iterator at container end.";
            throw std::out_of_range(ss.str());
        }

        float scale = this->sequence->getScale();

        this->tempNode = Node{minParentIdx};
        if (!this->isAtEnd(this->itrList.at(0)) &&
            (this->itrList.at(0).first->parentIdx == minParentIdx)) {
            this->tempNode += *this->itrList.at(0).first * scale;
        } else {
            this->tempNode += Node{minParentIdx} * scale;
        }

        if (!this->isAtEnd(this->itrList.at(1)) &&
            (this->itrList.at(1).first->parentIdx == minParentIdx)) {
            this->tempNode += *this->itrList.at(1).first * (1.0f - scale);
        } else {
            this->tempNode += Node{minParentIdx} * (1.0f - scale);
        }

        return this->tempNode;
    }

    bool SequenceIterator::operator!=(SequenceIterator otherItr) {
        return ((this->itrList.at(0).first != otherItr.itrList.at(0).first) ||
            (this->itrList.at(1).first != otherItr.itrList.at(1).first) ||
            (this->sequence != otherItr.sequence));
    }

    size_t SequenceIterator::getMinParent(void) {
        size_t minParent = -1u;

        for (auto& itrPair : this->itrList) {
            if (this->isAtEnd(itrPair)) {
                continue;
            }

            if (itrPair.first->parentIdx < minParent) {
                minParent = itrPair.first->parentIdx;
            }
        }

        return minParent;
    }

    bool SequenceIterator::isAtEnd(const std::pair<Offset::iterator, Offset::iterator>& itrPair) {
        return (itrPair.first == itrPair.second);
    }

    Sequence::Sequence(void) noexcept {}

    Sequence::Sequence(const std::vector<std::string> &offsetList,
                       const std::vector<float> &offsetTimeStamps_s,
                       const std::map<std::string, Offset> &offsetMap) {

        // Must be at least one Offset state and number of timestamps and Offsets equal.
        if ((offsetList.size() != offsetTimeStamps_s.size()) || !offsetList.size()) {
            std::stringstream ss;
            ss << "Invalid Sequence initialisation, bad number of Offsets and time points. "
               << offsetList.size() << " Offsets and " << offsetTimeStamps_s.size() << " times.";
            throw std::invalid_argument(ss.str());
        }

        // Timestamps must be strictly increasing.
        this->duration_s = offsetTimeStamps_s.at(offsetTimeStamps_s.size() - 1);
        float tempTime_s = offsetTimeStamps_s.at(0);
        for (size_t idx = 1; idx < offsetTimeStamps_s.size(); ++idx) {
            if (offsetTimeStamps_s.at(idx) <= tempTime_s) {
                std::stringstream ss;
                ss << "Invalid time points for Sequence initialisation. Must be strictly increasing. "
                   << "Time: " << tempTime_s << "s before time: " << offsetTimeStamps_s.at(idx);
                throw std::invalid_argument(ss.str());
            }
        }

        // If not an Offset at time 0s, insert default no Offset state.
        if (offsetTimeStamps_s.at(0) != 0.0f) {
            this->sequences.reserve(offsetList.size() + 1);
            sequences.push_back({0.0f, Offset{}});
        } else {
            this->sequences.reserve(offsetList.size());
        }

        this->sequences.reserve(offsetList.size());

        for (size_t idx = 0; idx < offsetList.size(); ++idx) {
            auto elem = offsetMap.find(offsetList.at(idx));
            if (elem == offsetMap.end()) {
                std::stringstream ss;
                ss << "Missing Offset named '" << offsetList.at(idx) << "'";
                throw std::invalid_argument(ss.str());
            }

            sequences.push_back({offsetTimeStamps_s.at(idx),
                offsetMap.at(offsetList.at(idx))});
        }
    }

    size_t Sequence::getIdx(float time_s) const {
        size_t startIdx = 0;

        if (time_s <= 0.0) {
            return this->sequences.size() - 1;
        }
        if (time_s >= this->duration_s) {
            return this->sequences.size() - 1;
        }

        for (size_t index = 0; index < this->sequences.size(); ++index) {
            if (sequences.at(index).time_s > time_s) {
                break;
            } else {
                startIdx = index;
            }
        }

        return startIdx;
    }

    float Sequence::getDuration(void) const noexcept {
        return this->duration_s;
    }

    float Sequence::getScale(void) const noexcept {
        float time_s = this->currentTime_s;

        if (time_s <= 0.0) {
            return 1.0f;
        }
        if (time_s >= this->duration_s) {
            return 1.0f;
        }

        size_t startIdx = getIdx(time_s);
        size_t nextIdx = (startIdx + 1) % sequences.size();

        return (sequences.at(nextIdx).time_s - time_s) /
            (sequences.at(nextIdx).time_s - sequences.at(startIdx).time_s);
    }

    void Sequence::reset(void) noexcept {
        this->currentTime_s = 0.0f;
        this->currentIndex = 0u;
    }

    void Sequence::update(float deltaTime_s) noexcept {
        this->currentTime_s += deltaTime_s;

        // still further offsets to transition to
        if (this->currentIndex < (this->sequences.size() - 1)) {
            if (this->currentTime_s >= (this->sequences.at(this->currentIndex + 1).time_s)) {
                ++this->currentIndex;
            }
        }
    }

    bool Sequence::isComplete(void) const noexcept {
        return (this->currentTime_s > this->duration_s);
    }

    Sequence::iterator Sequence::begin(void) {
        size_t nextIndex = (this->currentIndex + 1) % this->sequences.size();

        std::array<std::pair<Offset::iterator, Offset::iterator>, 2> itrList = {
            std::make_pair(sequences.at(this->currentIndex).offset.begin(),
                           sequences.at(this->currentIndex).offset.end()),
            std::make_pair(sequences.at(nextIndex).offset.begin(),
                           sequences.at(nextIndex).offset.end())
        };

        return SequenceIterator{itrList, this};
    }

    Sequence::iterator Sequence::end(void) {
        size_t nextIndex = (this->currentIndex + 1) % this->sequences.size();

        std::array<std::pair<Offset::iterator, Offset::iterator>, 2> itrList = {
            std::make_pair(sequences.at(this->currentIndex).offset.end(),
                           sequences.at(this->currentIndex).offset.end()),
            std::make_pair(sequences.at(nextIndex).offset.end(),
                           sequences.at(nextIndex).offset.end())
        };

        return SequenceIterator{itrList, this};
    }
};
