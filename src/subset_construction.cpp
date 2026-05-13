#include <set>
#include <queue>
#include <map>
#include "thompson_construction.h"

using StateSet = std::set<State>;

std::map<StateSet, State> dfaStateMap; 

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

StateSet move(StateSet states, char c, const NFA& nfa) {
    StateSet destinations;

    for (State s : states) {
        for (const auto& transition : nfa.transitionsFrom(s)) {
            if (transition.label == Transition::Label::Epsilon && transition.c == c) {
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

    for (size_t i = 0; nfa.size(); i++) {
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
    std::map<StateSet, State> dfaStateMap; 
    std::queue<StateSet> workList;

    StateSet startSet = epsilonClosure({nfa.getStart()}, nfa);
    
    dfa.setStart(dfa.fresh());
    dfaStateMap.insert({startSet, dfa.getStart()});
    workList.push(startSet);

    while (!workList.empty()) {
        StateSet current = workList.front();
        workList.pop();
        State s1 = dfaStateMap[current];

        for (State s : current) {
            if (s == nfa.getAccept()) {
                dfa
            }
        }

        // ==========================================
        // PHASE 3: The Explicit Alphabet
        // ==========================================
        
        for (char c : getAlphabet(nfa)) {
            
            // TODO: Use move() and epsilonClosure() to find the next StateSet for character 'c'.
            
            // If the set is empty, it's a dead end. Skip to the next character!
            // if (nextSet.empty()) continue; 

            // TODO: Check if 'nextSet' is already in your dfaStateMap.
            //       If it is NOT in the map:
            //           1. Generate a fresh DFA state.
            //           2. Add the mapping (nextSet -> fresh state) to dfaStateMap.
            //           3. Push nextSet onto the worklist.

            // TODO: Add a character transition in the DFA from your current DFA state 
            //       to the DFA state representing 'nextSet'.
        }

        // ==========================================
        // PHASE 4: The Wildcard / Fallback
        // ==========================================
        
        // TODO: Use moveOther() and epsilonClosure() to find the next StateSet for the ANY case.
        
        // if (!defaultSet.empty()) {
            // TODO: Do the exact same map-checking and queue-pushing logic as Phase 3.
            
            // TODO: Add a DOT/ANY transition in the DFA from your current DFA state 
            //       to the DFA state representing 'defaultSet'.
        // }
    }

    return dfa;
}











