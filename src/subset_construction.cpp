#include <set>
#include <queue>
#include <map>
#include "thompson_construction.h"

using StateSet = std::set<State>;

StateSet epsilonClosure(StateSet states, const NFA& nfa) {
    std::vector<State> stack;
    StateSet closure = states;

    for (State s : states) {
        stack.push_back(s);
    }

    while (!stack.empty()) {
        State current = stack.back();
        stack.pop_back();

        for (const auto& transition : nfa.transitionsFrom(current)) {
            if (transition.label == Transition::Label::Epsilon) {
                if (closure.find(transition.destination) == closure.end()) {
                    closure.insert(transition.destination);
                    stack.push_back(transition.destination);
                }
            }
        }
    }
    return closure;
}

StateSet moveAnyChar(StateSet states, const NFA& nfa) {
    StateSet destinations;

    for (State s : states) {
        for (const auto& transition : nfa.transitionsFrom(s)) {
            if (transition.label == Transition::Label::AnyChar) {
                destinations.insert(transition.destination);
            }
        }
    }
    return destinations;
}

StateSet move(StateSet states, char c, const NFA& nfa) {
    StateSet destinations;

    for (State s : states) {
        for (const auto& transition : nfa.transitionsFrom(s)) {
            if (transition.label == Transition::Label::Char && transition.c == c) {
                destinations.insert(transition.destination);
            } else if (transition.label == Transition::Label::AnyChar) {
                destinations.insert(transition.destination);
            }
        }
    }
    return destinations;
}

std::set<char> getAlphabet(const NFA& nfa) {
    std::set<char> alphabet;

    for (size_t i = 0; i < nfa.size(); i++) {
        for (const auto& transition : nfa.transitionsFrom(i)) {
            if (transition.label == Transition::Label::Char) {
                alphabet.insert(transition.c);
            }
        }
    }
    return alphabet;
}

NFA buildDFA(const NFA& nfa) {
    NFA dfa;
    std::map<StateSet, State> stateMap; 
    std::queue<StateSet> workList;

    StateSet startSet = epsilonClosure({nfa.getStart()}, nfa);
    
    dfa.setStart(dfa.fresh());
    stateMap.insert({startSet, dfa.getStart()});
    workList.push(startSet);

    while (!workList.empty()) {
        StateSet current = workList.front();
        workList.pop();
        State dfaState = stateMap[current];

        for (State s : current) {
            if (nfa.getAccept().count(s)) {
                dfa.setAccept(dfaState);
                break;
            }
        }

        for (char c : getAlphabet(nfa)) {
            StateSet nextSet = epsilonClosure(move(current, c, nfa), nfa);
            if (nextSet.empty()) continue;

            if (stateMap.find(nextSet) == stateMap.end()) {
                stateMap.insert({nextSet, dfa.fresh()}); 
                workList.push(nextSet);
            }
            dfa.addCharTransition(dfaState, c, stateMap[nextSet]);
        }

        StateSet nextSet = epsilonClosure(moveAnyChar(current, nfa), nfa);
        if (!nextSet.empty()) {
            if (stateMap.find(nextSet) == stateMap.end()) {
                stateMap.insert({nextSet, dfa.fresh()}); 
                workList.push(nextSet);
            }
            dfa.addDotTransition(dfaState, stateMap[nextSet]);
        }
    }
    return dfa;
}
