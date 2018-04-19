/*
 * Animation.hpp
 *
 *  Created on: Apr 3, 2018
 *      Author: Simon
 */

#ifndef SRC_ANIMATION_HPP_
#define SRC_ANIMATION_HPP_

#include <WorldManager.hpp>
#include <UtilityManager.hpp>

#include "Sequence.hpp"

namespace mod {
    enum class AnimationState {
        TRANSITION_IN,
        LOOP,
        TRANSITION_OUT
    };

    struct AnimationDefinition {
        std::string inSequenceName;
        std::vector<std::string> loopSequenceNames;
        std::vector<std::string> outSequenceNames;
    };

    std::istream& operator>>(std::istream& is, AnimationDefinition& animDef);

    /*
     * Class to define an animation action for a model. Starts with a Sequence to transition
     * in, then a list of Sequences looped through in turn during the animation and finally
     * one last Sequence out to return to the base state.
     */
    class Animation : protected UtilityManager {
    public:
        using iterator = Sequence::iterator;

        Animation(std::string inSeqName,
                  std::vector<std::string> &loopSeqNames,
                  std::vector<std::string> &outSeqNames,
                  std::map<std::string, mod::Sequence> &seqMap);

        void start(void);
        void update(float deltaTime_s);
        void flagEnd(void);

        iterator begin();
        iterator end();

    private:
        mod::Sequence transitionInSequence;
        std::vector<mod::Sequence> loopSequences;
        std::vector<mod::Sequence> transitionOutSequences;

        AnimationState state{AnimationState::TRANSITION_IN};
        size_t sequenceIndex{0u};

        bool finishing{false};
        bool complete{true};

        Sequence& currentSequence(void);
    };
};

#endif /* SRC_ANIMATION_HPP_ */
