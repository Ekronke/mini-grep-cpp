#include <string>
#include <iomanip>
#include <iostream>

class Token {
    public:
        enum class Kind {
            Unexpected,
            Literal,
            Union,
            Concatination,
            Closure,
            OneOrMore,
            ZeroOrOne,
            LeftParenthesis,
            RightParenthesis,
            Dot,
            End,
        };

        Token(Kind kind) noexcept : m_kind{kind} {}

        Token(Kind kind, const char* beg, std::size_t len) noexcept
            : m_kind{kind}, m_lexeme(beg, len) {}

        Token(Kind kind, const char* beg, const char* end) noexcept
            : m_kind{kind}, m_lexeme(beg, std::distance(beg, end)) {}

        Kind kind() const noexcept { return m_kind; }
        
        void kind(Kind kind) noexcept { m_kind = kind; }

        bool is(Kind kind) const noexcept { return m_kind == kind; }

        bool is_not(Kind kind) const noexcept { return m_kind != kind; }

        bool is_one_of(Kind k1, Kind k2) const noexcept { return is(k1) || is(k2); }

        template <typename... Ts>
        bool is_one_of(Kind k1, Kind k2, Ts... ks) const noexcept {
            return is(k1) || is_one_of(k2, ks...);
        }

        std::string_view lexeme() const noexcept { return m_lexeme; }

        void lexeme(std::string_view lexeme) noexcept {
            m_lexeme = std::move(lexeme);
        }

    private:
        Kind m_kind{};
        std::string_view m_lexeme{};

};

class Lexer {
    public:
        Lexer(const char* beg) noexcept : m_beg{beg} {}

        Token next() noexcept;

    private:
        Token literal() noexcept;
        Token atom(Token::Kind) noexcept;

        char peek() const noexcept { return *m_beg; }
        char get() noexcept { return *m_beg++; }

        const char* m_beg = nullptr;
};

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
            "Concatination",
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

int main(void) {
    auto code = 
        "ab.cl(av)?d+";

    Lexer lex(code);
    for (auto token = lex.next();
        not token.is_one_of(Token::Kind::End, Token::Kind::Unexpected);
        token = lex.next()) {
            std::cout << std::setw(12) << token.kind() << " |" << token.lexeme()
            << "|\n";
    }
}
