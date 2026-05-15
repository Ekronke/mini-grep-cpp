#include "parser.h"
#include "thompson_construction.h"
#include "subset_construction.h"
#include "table_filling.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

int main() {
    std::string file_name;
    std::cout << "Enter file name: ";
    std::cin >> file_name;

    std::ifstream inFil(file_name);  

    if (inFil) {  
        std::vector<double> numbers_list;

        std::string alphabetStr;
        std::string regExprStr;
        std::string line;

        std::getline(inFil, alphabetStr);
        std::getline(inFil, regExprStr);

        // Lab tip
        regExprStr = ".*(" + regExprStr + ")";
        
        std::vector<char> alphabet(alphabetStr.begin(), alphabetStr.end());
        const char* regExpr = regExprStr.c_str();

        // PARSE
        Parser parser(regExpr);
        auto ast = parser.parse();
        if (!ast) {
            std::cerr << "parse error: " << ast.error().message << "\n";
            return 1;
        }

        // THOMPSON CONSTRUCTION
        std::cout << "THOMPSON CONSTRUCTION \n";
        NFA nfa = buildNFA(**ast);   
        nfa.printAutomata();
        
        // ε-NFA to DFA
        std::cout << "ε-NFA to DFA \n";
        NFA dfa = buildDFA(nfa);
        dfa.printAutomata();

        // DFA minimazation
        std::cout << "DFA to Minimal DFA \n";
        NFA minimalDFA = buildMinimalDFA(dfa);
        minimalDFA.printAutomata();

        // grep 
        std::cout << "Matching lines: \n";

        size_t n = minimalDFA.size();
        std::vector<std::map<char, State>> charToState(n);
        for (size_t i = 0; i < n; i++) {
            for (const auto& transition : minimalDFA.transitionsFrom(i)) {
                if (transition.label == Transition::Label::Char) {
                    charToState[i][transition.c] = transition.destination;
                }
            }
        }
        std::vector<std::optional<State>> anyCharToState(n);
        for (size_t i = 0; i < n; i++) {
            for (const auto& transition : minimalDFA.transitionsFrom(i)) {
                if (transition.label == Transition::Label::AnyChar) {
                    anyCharToState[i] = transition.destination;
                }
            }
        }

        State start = minimalDFA.getStart();
        std::set<State> accepting = minimalDFA.getAccept();
        while (inFil) { 
            std::getline(inFil, line);
            State current = start;

            for (auto it = line.begin(); it != line.end(); ++it) {
                char c = *it;
                if (charToState[current].find(c) != charToState[current].end()) {
                    current = charToState[current].find(c)->second;
                } else if (anyCharToState[current].has_value()) {
                    current = anyCharToState[current].value();
                } else {
                    break;
                }
                if (accepting.contains(current)) {
                    std::cout << line << '\n';
                    break;
                }
            }
        }

        std::cout << "\n";
    } else {
        std::cout << "File could not be opened!!";
    }
    
    return 0;
}

