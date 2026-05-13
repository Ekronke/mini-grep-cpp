#include "parser.h"
#include "thompson_construction.h"

#include <iostream>
#include <variant>

State NFA::fresh() {
    transitions.emplace_back();
    return transitions.size() - 1;
}

void NFA::addCharTransition(State from, char c, State to) {
    transitions[from].push_back({Transition::Label::Char, c, to});
}

void NFA::addEpsilonTransition(State from, State to) {
    transitions[from].push_back({Transition::Label::Epsilon, '\0', to});
}

void NFA::addDotTransition(State from, State to) {
    transitions[from].push_back({Transition::Label::AnyChar, '\1', to});
}

void NFA::printAutomata() {
    std::cout << "Start: " << getStart() << '\n' << "Accepting: ";
    for (auto& s : getAccept()) {
        std::cout << s << ' ';
    }
    std::cout << '\n';

    for (size_t i = 0; i < size(); i++) {
        std::cout << i;
        for (auto &transition : transitionsFrom(i)) {
            std::cout << " --> ";
            
            if (transition.label == Transition::Label::Epsilon) {
                std::cout << "ε";    
            } else if (transition.label == Transition::Label::AnyChar) {
                std::cout << "ANY";
            } else {
                std::cout << transition.c; 
            }
            
            std::cout << " --> " << transition.destination;
        }
        std::cout << '\n';
    }
}

namespace {

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

Fragment build(NFA& nfa, const Node& node);

Fragment buildLiteral(NFA& nfa, const LiteralNode& n) {
    State s = nfa.fresh();
    State f = nfa.fresh();
    nfa.addCharTransition(s, n.literal, f);
    return {s, f};
}

Fragment buildDot(NFA& nfa) {
    State s = nfa.fresh();
    State f = nfa.fresh();
    nfa.addDotTransition(s,f); 
    return {s, f};
}

Fragment buildConcat(NFA& nfa, const ConcatNode& n) {
    Fragment left = build(nfa, *n.left);
    Fragment right = build(nfa, *n.right);
    nfa.addEpsilonTransition(left.accept, right.start);
    return {left.start, right.accept};
}

Fragment buildUnion(NFA& nfa, const UnionNode& n) {
    State s = nfa.fresh();
    State f = nfa.fresh();
    Fragment left = build(nfa, *n.left);
    Fragment right = build(nfa, *n.right);
    nfa.addEpsilonTransition(s, left.start);
    nfa.addEpsilonTransition(s, right.start);
    nfa.addEpsilonTransition(left.accept, f);
    nfa.addEpsilonTransition(right.accept, f);
    return {s, f};
}

Fragment buildClosure(NFA& nfa, const ClosureNode& n) {
    State s = nfa.fresh();
    State f = nfa.fresh();
    Fragment inner = build(nfa, *n.child);
    nfa.addEpsilonTransition(s, inner.start);
    nfa.addEpsilonTransition(s, f);
    nfa.addEpsilonTransition(inner.accept, inner.start);
    nfa.addEpsilonTransition(inner.accept, f);

    return {s, f};
}

Fragment buildOneOrMore(NFA& nfa, const OneOrMoreNode& n) {
    State s = nfa.fresh();
    State f = nfa.fresh();
    Fragment inner = build(nfa, *n.child);
    nfa.addEpsilonTransition(s, inner.start);
    nfa.addEpsilonTransition(inner.accept, inner.start);
    nfa.addEpsilonTransition(inner.accept, f);

    return {s, f};
}

Fragment buildZeroOrOne(NFA& nfa, const ZeroOrOneNode& n) {
    State s = nfa.fresh();
    State f = nfa.fresh();
    Fragment inner = build(nfa, *n.child);
    nfa.addEpsilonTransition(s, inner.start);
    nfa.addEpsilonTransition(s, f);
    nfa.addEpsilonTransition(inner.accept, f);

    return {s, f};
}

Fragment build(NFA& nfa, const Node& node) {
    return std::visit(overloaded{
        [&](const LiteralNode& n)   { return buildLiteral(nfa, n); },
        [&](const DotNode&)         { return buildDot(nfa); },
        [&](const ConcatNode& n)    { return buildConcat(nfa, n); },
        [&](const UnionNode& n)     { return buildUnion(nfa, n); },
        [&](const ClosureNode& n)   { return buildClosure(nfa, n); },
        [&](const OneOrMoreNode& n) { return buildOneOrMore(nfa, n); },
        [&](const ZeroOrOneNode& n) { return buildZeroOrOne(nfa, n); },
    }, node.kind);
}

} // namespace

NFA buildNFA(const Node& ast) {
    NFA nfa;
    Fragment frag = build(nfa, ast);
    nfa.setStart(frag.start);
    nfa.setAccept(frag.accept);
    return nfa;
}
