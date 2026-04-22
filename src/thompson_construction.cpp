#include "parser.h"
#include "thompson_construction.h"

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

Fragment bulidDot(NFA& nfa) {
    State s = nfa.fresh();
    State f = nfa.fresh();
    nfa.addDotTransition(s,f); 
    return {s, f};
}

Fragment buildConcat(NFA& nfa, const ConcatNode& n) {
    Fragment left = nfa.build(nfa, *n.left);
    Fragment right = nfa.build(nfa, *n.right);
    nfa.addEpsilonTransition(left.accept, right.start);
    return {left.accept, right.start};
}

Fragment buildUnion(NFA& nfa, const UnionNode& n) {
    State s = nfa.fresh();
    State f = nfa.fresh();
    Fragment left = nfa.build(nfa, *n.left);
    Fragment right = nfa.build(nfa, *n.right);
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
