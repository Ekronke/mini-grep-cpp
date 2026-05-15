#pragma once

#include "thompson_construction.h"

std::set<char> getAlphabet(const NFA& nfa);
NFA buildDFA(const NFA& nfa);
