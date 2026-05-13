#include "lexer.h"

#include <iomanip>
#include <iostream>


bool is_valid_char(char c) noexcept {
    switch (c) {
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return true;
        default:
            return false;
    }
}

Token Lexer::atom(Token::Kind kind) noexcept { 
    return Token(kind, m_beg++, 1); 
}

Token Lexer::next() noexcept {
    switch (peek()) {
        case '\0':
            return Token(Token::Kind::End, m_beg, 1);
        default:
            return atom(Token::Kind::Unexpected);
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return literal();
        case '|': return atom(Token::Kind::Union);
        case '*': return atom(Token::Kind::Closure);
        case '+': return atom(Token::Kind::OneOrMore);
        case '?': return atom(Token::Kind::ZeroOrOne);
        case '(': return atom(Token::Kind::LeftParenthesis);
        case ')': return atom(Token::Kind::RightParenthesis);
        case '.': return atom(Token::Kind::Dot);
    }
}

Token Lexer::literal() noexcept {
    const char* start = m_beg;
    get();
    return Token(Token::Kind::Literal, start, m_beg);
}

std::ostream& operator<<(std::ostream& os, const Token::Kind& kind) {
    static const char* const names[]{
            "Unexpected",
            "Literal",
            "Union",
            "Closure",
            "OneOrMore",
            "ZeroOrOne",
            "LeftParenthesis",
            "RightParenthesis",
            "Dot",
            "End",
    };
    return os << names[static_cast<int>(kind)];
}

// int main(void) {
//     auto code = 
//         "ab.cl(av)?d+";
//
//     Lexer lex(code);
//     for (auto token = lex.next();
//         not token.is_one_of(Token::Kind::End, Token::Kind::Unexpected);
//         token = lex.next()) {
//             std::cout << std::setw(12) << token.kind() << " |" << token.lexeme()
//             << "|\n";
//     }
// }
