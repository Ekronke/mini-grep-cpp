#pragma once

#include "parser.h"

#include <set>
#include <cstddef>
#include <vector>

using State = std::size_t;

struct Transition {
    enum class Label { Char, Epsilon, AnyChar } label;
    char c;
    State destination;
};

struct Fragment {
    State start;
    State accept;
};

class NFA {
    public:
        State fresh();
        void addCharTransition(State from, char c, State to);
        void addEpsilonTransition(State from, State to);
        void addDotTransition(State from, State to);
        void printAutomata();

        void setStart(State s) noexcept  { start = s; }
        void setAccept(State a) noexcept { accept.insert(a); }

        State getStart()  const noexcept { return start; }
        std::set<State> getAccept() const noexcept { return accept; }
        std::size_t size() const noexcept { return transitions.size(); }

        const std::vector<Transition>& transitionsFrom(State s) const noexcept {
            return transitions[s];
        }

    private:
        std::vector<std::vector<Transition>> transitions;
        State start  = 0;
        std::set<State> accept = {};
};

NFA buildNFA(const Node& ast);
