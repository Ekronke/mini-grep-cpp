#include <algorithm>
#include <vector>
#include <map>
#include "thompson_construction.h"
#include "subset_construction.h"

std::vector<std::vector<char>> tableFilling(const NFA& dfa) {
    size_t n = dfa.size();
    std::vector<std::vector<char>> table(n, std::vector<char>(n, 0));
    std::set<State> accepting = dfa.getAccept();

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < i; j++) {
            if (accepting.contains(i) != accepting.contains(j)) {
                table[i][j] = 1;
            }
        }
    }

    std::vector<std::map<char, State>> charToState(n);
    for (size_t i = 0; i < n; i++) {
        for (const auto& transition : dfa.transitionsFrom(i)) {
            if (transition.label == Transition::Label::Char) {
                charToState[i][transition.c] = transition.destination;
            }
        }
    }
    
    std::set<char> alphabet = getAlphabet(dfa);
    bool madeChanges = 1;
    while (madeChanges) {
        madeChanges = 0;
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < i; j++) {
                if (table[i][j] == 1) continue;

                for (char c : alphabet) {
                    State d1 = charToState[i][c];
                    State d2 = charToState[j][c];

                    if (d1 == d2) continue;

                    if (d1 < d2) {
                        std::swap(d1, d2);
                    }

                    if (table[d1][d2] == 1) {
                        table[i][j] = 1;
                        madeChanges = 1;
                        break;
                    }
                }
            }
        }
    }
    return table;
}

NFA buildMinimalDFA(const NFA& dfa) {
    std::vector<std::vector<char>> table = tableFilling(dfa);
    size_t n = dfa.size();

    std::vector<State> repr(n);
    for (size_t i = 0; i < n; i++) {
        repr[i] = i;
        for (size_t j = 0; j < i; j++) {
            if (table[i][j] == 0) {
                repr[i] = repr[j];
                break;
            }
        }
    }

    std::map<State, State> reprToNew;
    NFA minimalDFA;
    for (size_t i = 0; i < n; i++) {
        if (repr[i] == i) {
            reprToNew[i] = minimalDFA.fresh();
        }
    }

    minimalDFA.setStart(reprToNew[repr[dfa.getStart()]]);

    std::set<State> accepting = dfa.getAccept();
    for (size_t i = 0; i < n; i++) {
        if (repr[i] != i) continue;
        State newState = reprToNew[i];
        if (accepting.count(i)) {
            minimalDFA.setAccept(newState);
        }
        for (const auto& transition : dfa.transitionsFrom(i)) {
            State dest = reprToNew[repr[transition.destination]];
            if (transition.label == Transition::Label::Char) {
                minimalDFA.addCharTransition(newState, transition.c, dest);
            } else if (transition.label == Transition::Label::AnyChar) {
                minimalDFA.addDotTransition(newState, dest);
            }
        }
    }

    return minimalDFA;
}
