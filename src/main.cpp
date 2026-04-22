#include "lexer.h"
#include "parser.h"
#include "thompson_construction.h"

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

        Parser parser(regExpr);
        auto ast = parser.parse();
        if (!ast) {
            std::cerr << "parse error: " << ast.error().message << "\n";
            return 1;
        }
        NFA nfa = buildNFA(**ast);   


        while (inFil) { 
            std::getline(inFil, line);
        }

        std::cout << "\n";
    } else {
        std::cout << "File could not be opened!!";
    }
}

