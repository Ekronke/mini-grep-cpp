#pragma once

#include <cstddef>
#include <iterator>
#include <string_view>
#include <utility>

class Token {
    public:
        enum class Kind {
            Unexpected,
            Literal,
            Union,
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

bool is_valid_char(char c) noexcept;
