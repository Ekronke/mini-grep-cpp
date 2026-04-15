#include "lexer.h"
#include "parser.h"

#include <expected>
#include <variant>
#include <memory>

/* *
 * PUBLIC 
 * */

Parser::ParseResult Parser::parse() noexcept {
    auto root = parseUnion();
    if (!root) {
        return std::unexpected(root.error());
    }

    if (token.is_not(Token::Kind::End)) {
        return std::unexpected(ParseError{
            "Unexpected token after expression.",
            token.kind(),
            SourceSpan{0,1}
        });
    }
    return root;
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

std::expected<void, ParseError> Parser::expect(Token::Kind expected_token_type) noexcept {
    if (token.is(expected_token_type)) {
        consume();
        return {};
    }
    return std::unexpected(ParseError{
        "Unexpected token",
        token.kind(),
        SourceSpan{0, 1}
    });
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
        auto inner = parseUnion();
        if (!inner) {
            return std::unexpected(inner.error());
        }
        auto result = expect(Token::Kind::RightParenthesis);
        if (!result) {
            return std::unexpected(result.error());
        }
        return inner;
    }
    return std::unexpected(ParseError{
        "Unexpected token after expression.",
        token.kind(),
        SourceSpan{0,1}
    });
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
    auto left = parseQuantifier();
    if (!left) {
        return std::unexpected(left.error());
    }

    if(token.is_one_of(Token::Kind::Literal, Token::Kind::Dot, Token::Kind::LeftParenthesis)) {
        auto right = parseConcat();
        if (!right) {
            return std::unexpected(right.error());
        }

        auto node = std::make_unique<Node>(Node{
            SourceSpan{0, 1},
            ConcatNode{std::move(*left), std::move(*right)}
        });
        return node;
    }
    return left;
}

Parser::ParseResult Parser::parseUnion() noexcept {
    auto left = parseConcat();
    if (!left) {
        return std::unexpected(left.error());
    }
    
    if(token.is(Token::Kind::Union)) {
        consume();
        auto right = parseUnion();
        if (!right) {
            return std::unexpected(right.error());
        }

        auto node = std::make_unique<Node>(Node{
            SourceSpan{0, 1},
            UnionNode{std::move(*left), std::move(*right)}
        });
        return node;
    }
    return left;
}

