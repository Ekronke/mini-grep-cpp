#include "lexer.h"
#include "parser.h"

#include <expected>
#include <variant>
#include <memory>

/* *
 * PUBLIC 
 * */

Parser::ParseResult Parser::parse() noexcept {
}

/* *
 * PRIVATE
 * */

void Parser::consume() noexcept {
    token = lexer.next();
}

bool Parser::match(Token::Kind expected_token_type) noexcept {
    if (token.is(expected_token_type)) {
        consume();
        return true;
    } else {
        return false;
    }
}

Parser::ParseResult Parser::parsePrimary() noexcept {
    if (token.is(Token::Kind::Literal)) {
        char c = token.lexeme()[0];
        auto node = std::make_unique<Node>(Node{
            SourceSpan{0, 1},
            LiteralNode{c}
        });
        consume();
        return node;
    } else if (token.is(Token::Kind::Dot)) {
        auto node = std::make_unique<Node>(Node{
            SourceSpan{0, 1},
            DotNode{}
        });
        consume();
        return node;
    } else if (token.is(Token::Kind::LeftParenthesis)) {
        consume();
        auto result = expect(Token::Kind::RightParenthesis);
        if (!result) {
            return std::unexpected(result.error());
        }
        parseUnion();
    }
}

Parser::ParseResult Parser::parseQuantifier() noexcept {
    auto primary = parsePrimary();
    if (!primary) {
        return std::unexpected(primary.error());
    }

    if (token.is(Token::Kind::Closure)) {
        auto node = std::make_unique<Node>(Node{
            SourceSpan{0, 1},
            ClosureNode{std::move(*primary)}
        });
        consume();
        return node;
    } else if (token.is(Token::Kind::OneOrMore)) {
        auto node = std::make_unique<Node>(Node{
            SourceSpan{0, 1},
            OneOrMoreNode{std::move(*primary)}
        });
        consume();
        return node;
    } else if (token.is(Token::Kind::ZeroOrOne)) {
        auto node = std::make_unique<Node>(Node{
            SourceSpan{0, 1},
            ZeroOrOneNode{std::move(*primary)}
        });
        consume();
        return node;
    }
    return primary;
}

Parser::ParseResult Parser::parseConcat() noexcept {
}


