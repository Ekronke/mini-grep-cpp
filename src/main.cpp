#include "parser.h"
#include "thompson_construction.h"
#include "subset_construction.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

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


        // grep 

        while (inFil) { 
            std::getline(inFil, line);
        }

        std::cout << "\n";
    } else {
        std::cout << "File could not be opened!!";
    }
    
    return 0;
}

