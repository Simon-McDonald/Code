/*
 * Animation.cpp
 *
 *  Created on: Apr 3, 2018
 *      Author: Simon
 */

#include <cassert>

#include "Sequence.hpp"
#include "Animation.hpp"

namespace mod {
    std::istream& operator>>(std::istream& is, AnimationDefinition& animDef) {
        for (std::string line;
             (is.peek() != '\n') && std::getline(is, line); ) {

            std::string title, listString;
            cvt::splitAtDelim(line, '=', title, listString);

            if (!title.compare("transition_in")) {
                animDef.inSequenceName = listString;
            } else if (!title.compare("loop")) {
                animDef.loopSequenceNames = cvt::splitToList<std::string>(listString);
            } else if (!title.compare("transition_out")) {
                animDef.outSequenceNames = cvt::splitToList<std::string>(listString);
            }
        }

        return is;
    }

    Animation::Animation(std::string inSeqName,
                         std::vector<std::string> &loopSeqNames,
                         std::vector<std::string> &outSeqNames,
                         std::map<std::string, mod::Sequence> &seqMap) {

        if ((loopSeqNames.size() != outSeqNames.size()) || !loopSeqNames.size()) {
            std::stringstream ss;
            ss << "Invalid Sequence specification for animation. Number of loop sequences "
               << loopSeqNames.size() << " and " << outSeqNames.size() << " exit sequences.";
            throw std::invalid_argument(ss.str());
        }

        auto elem = seqMap.find(inSeqName);
        if (elem == seqMap.end()) {
            std::stringstream ss;
            ss << "Missing Sequence named '" << inSeqName << "' for transition in";
            throw std::invalid_argument(ss.str());
        }
        this->transitionInSequence = seqMap.at(inSeqName);

        this->loopSequences.reserve(loopSeqNames.size());
        this->transitionOutSequences.reserve(outSeqNames.size());

        for (const auto& seqName : loopSeqNames) {
            auto elem = seqMap.find(seqName);
            if (elem == seqMap.end()) {
                std::stringstream ss;
                ss << "Missing Sequence named '" << seqName << "' for loop";
                throw std::invalid_argument(ss.str());
            }

            this->loopSequences.push_back(seqMap.at(seqName));
        }
        for (const auto& seqName : outSeqNames) {
            auto elem = seqMap.find(seqName);
            if (elem == seqMap.end()) {
                std::stringstream ss;
                ss << "Missing Sequence named '" << seqName << "' for transition out";
                throw std::invalid_argument(ss.str());
            }

            this->transitionOutSequences.push_back(seqMap.at(seqName));
        }
    }

    void Animation::start(void) {
        this->state = AnimationState::TRANSITION_IN;
        this->complete = false;
        this->finishing = false;
        this->sequenceIndex = 0;
        this->currentSequence().reset();
    }

    void Animation::update(float deltaTime_s) {
        if (this->complete) {
            return;
        }

        auto& currAnim = this->currentSequence();
        currAnim.update(deltaTime_s);

        if (currAnim.isComplete()) {
            switch (this->state) {
            case AnimationState::TRANSITION_IN:
                if (this->finishing) {
                    this->state = AnimationState::TRANSITION_OUT;
                    this->sequenceIndex = this->loopSequences.size() - 1;
                } else {
                    this->state = AnimationState::LOOP;
                }
                break;
            case AnimationState::LOOP:
                if (this->finishing) {
                    this->state = AnimationState::TRANSITION_OUT;
                } else {
                    this->sequenceIndex = (this->sequenceIndex + 1) % this->loopSequences.size();
                }
                break;
            case AnimationState::TRANSITION_OUT:
                this->state = AnimationState::TRANSITION_IN;
                this->complete = true;
                break;
            }

            this->currentSequence().reset();
        }
    }

    void Animation::flagEnd(void) {
        this->finishing = true;
    }

    Animation::iterator Animation::begin() {
        if (this->complete) {
            return this->end();
        }

        return this->currentSequence().begin();
    }

    Animation::iterator Animation::end() {
        return this->currentSequence().end();
    }

    Sequence& Animation::currentSequence(void) {
        switch (this->state) {
        case AnimationState::TRANSITION_IN:
            return transitionInSequence;
        case AnimationState::LOOP:
            return loopSequences.at(this->sequenceIndex);
        case AnimationState::TRANSITION_OUT:
        default: // avoid warning
            return transitionOutSequences.at(this->sequenceIndex);
        }
    }
};
