#pragma once

#include "lexer.h"
#include "parser.h"

#include <variant>
#include <memory>
#include <expected>
#include <string.h>

struct Node;
struct DotNode;
struct LiteralNode;
struct ConcatNode;
struct UnionNode;
struct ClosureNode;
struct OneOrMoreNode;
struct ZeroOrOneNode;

struct SourceSpan {
    std::size_t start;
    std::size_t length;
};

using NodeHandler = std::unique_ptr<const Node>;

struct DotNode {};

struct LiteralNode {
    char literal;
};

struct ConcatNode {
    NodeHandler left;
    NodeHandler right;
};

struct UnionNode {
    NodeHandler left;
    NodeHandler right;
};

struct ClosureNode {
    NodeHandler child; 
};

struct OneOrMoreNode {
    NodeHandler child;
};

struct ZeroOrOneNode {
    NodeHandler child;
};

struct Node {
    struct SourceSpan span;
    std::variant<LiteralNode,
                 DotNode,
                 ConcatNode, 
                 UnionNode,
                 ClosureNode,
                 OneOrMoreNode,
                 ZeroOrOneNode> kind; 
};

struct ParseError {
    std::string message;
    Token::Kind unexpected_token;
    SourceSpan span;
};

class Parser {
    public:
        using ParseResult = std::expected<NodeHandler, ParseError>;
        ParseResult parse() noexcept;

        Parser(const char* expr)
            : lexer{expr}
            , token{lexer.next()}
            {}

    private:
        Lexer lexer;
        Token token;
        

        void consume() noexcept;
        bool match(Token::Kind expected_token_type) noexcept;

        std::expected<void, ParseError> expect(Token::Kind expected_token_type) noexcept;

        // Recursive descent methods
        NodeHandler parsePrimary() noexcept;
        NodeHandler parseQuantifier() noexcept;
        NodeHandler parseConcat() noexcept;
        NodeHandler parseUnion() noexcept;
};
